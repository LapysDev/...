public sealed class Unit : UnityEngine.MonoBehaviour {
  public enum Kind : byte { _, Soldier, Zombie }

  /* … */
  public const uint  HEALTH_MAXIMUM                = 200u;
  public const uint  SOLDIER_HEALTH_MAXIMUM        = 120u;
  public const float TARGET_GROUP_DISTANCE_SQUARED = 4.0f;
  public const float TARGET_REACH_DISTANCE_SQUARED = 2.0f;
  public const uint  ZOMBIE_HEALTH_MAXIMUM         = 70u;

  public                                             float                   angularSpeed         = 15.0f;               // ⟶ In degrees°
  public                                             UnityEngine.GameObject? bulletPrefabrication = null;                // ⟶ For `Soldier`s only
  public                                             uint                    damage               = 0u;                  //
  [UnityEngine.Range(0, Unit.HEALTH_MAXIMUM)] public uint                    health               = Unit.HEALTH_MAXIMUM; //
  public                                             Unit.Kind               kind                 = Unit.Kind._;         //
  public                                             double                  reloadInterval       = 0.2;                 // ⟶ Reload ammo every ⅕ seconds
  public                                             float                   speed                = 2.5f;                //

  /* … */
  private void Awake() {
    UnityEngine.Debug.Log("Unexpected debug log.");
  }
}
  public sealed class UnitBaker : Unity.Entities.Baker<Unit> {
    public override void Bake(Unit unit) {
      Unity.Entities.Entity entity = this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic);

      // …
      this.AddComponent(entity, new UnitComponent() {angularSpeed = unit.angularSpeed, damage = unit.damage, health = unit.health, kind = unit.kind, speed = unit.speed, targetPosition = unit.transform.position});
      switch (unit.kind) {
        case Unit.Kind.Soldier: this.AddComponent(entity, new UnitComponent.SoldierComponent() {bulletPrefabrication = null == unit.bulletPrefabrication ? Unity.Entities.Entity.Null : this.GetEntity(unit.bulletPrefabrication, Unity.Entities.TransformUsageFlags.Dynamic), reloadInterval = unit.reloadInterval, reloadTimestamp = 0.0}); break;
        case Unit.Kind.Zombie : this.AddComponent(entity, new UnitComponent.ZombieComponent ()); break;
        default: break;
      }
    }
  }

  public struct UnitComponent : Unity.Entities.IComponentData {
    public partial struct SoldierComponent : Unity.Entities.IComponentData {
      public Unity.Entities.Entity bulletPrefabrication; // ⟶ Consider using a `::GetSingleton<PrefabsComponent>()`
      public double                reloadInterval;
      public double                reloadTimestamp;
    }

    public partial struct ZombieComponent : Unity.Entities.IComponentData {}

    /* … */
    public float                    angularSpeed;
    public uint                     damage;
    public uint                     health;
    public Unit.Kind                kind;
    public float                    speed;
    public Unity.Mathematics.float3 targetPosition;
  }

  [Unity.Burst.BurstCompile]
  [Unity.Entities.UpdateAfter(typeof(BulletSystem)), Unity.Entities.UpdateAfter(typeof(FinderSystem)), Unity.Entities.UpdateBefore(typeof(SelectableSystem))]
  // [Unity.Entities.UpdateInGroup(typeof(Unity.Entities.SimulationSystemGroup))]
  public partial struct UnitSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    private partial struct UnitJob : IJobEntity {
      public float deltaTime;

      /* … */
      public void Execute(in UnitComponent unit, ref Unity.Transforms.LocalTransform localTransform, ref Unity.Physics.PhysicsVelocity physicsVelocity) {
        Unity.Mathematics.float3 moveDirection = unit.targetPosition - localTransform.Position;

        // …
        if (Unit.TARGET_REACH_DISTANCE_SQUARED >= Unity.Mathematics.math.lengthsq(moveDirection))
          physicsVelocity.Linear = Unity.Mathematics.float3.zero;

        else {
          moveDirection            = Unity.Mathematics.math.normalize(moveDirection);
          localTransform .Rotation = Unity.Mathematics.math.slerp(localTransform.Rotation, Unity.Mathematics.quaternion.LookRotation(moveDirection, Unity.Mathematics.math.up()), this.deltaTime * unit.angularSpeed);
          physicsVelocity.Linear   = moveDirection * unit.speed;
        }

        physicsVelocity.Angular = Unity.Mathematics.float3.zero;
      }
    }

    /* … */
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      Unity.Entities.EntityCommandBuffer commands = new(Unity.Collections.Allocator.Temp); // ⟶ Alternative to using built-ins: `Unity.Entities.SystemAPI.GetSingleton<Unity.Entities.EndSimulationEntityCommandBufferSystem.Singleton>().CreateCommandBuffer(state.WorldUnmanaged)`
      Unity.Jobs.JobHandle               job      = new UnitJob() {deltaTime = Unity.Entities.SystemAPI.Time.DeltaTime}.ScheduleParallel(state.Dependency); // ⟶ Multi-threaded as opposed to `Unity.Entities.IJobEntity::Run()`

      // …
      job.Complete(); // ⟶ Pause main single threaded until the `job` completes

      foreach (var (finder, soldier, unit, localTransform) in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<FinderComponent>, Unity.Entities.RefRW<UnitComponent.SoldierComponent>, Unity.Entities.RefRO<UnitComponent>, Unity.Entities.RefRO<Unity.Transforms.LocalTransform>>())
      if (Unity.Entities.Entity.Null != finder.ValueRO.target && soldier.ValueRW.reloadInterval <= Unity.Entities.SystemAPI.Time.ElapsedTime - soldier.ValueRW.reloadTimestamp) {
        Unity.Entities.RefRW<UnitComponent> targetUnit = Unity.Entities.SystemAPI.GetComponentRW<UnitComponent>(finder.ValueRO.target);

        // …
        soldier.ValueRW.reloadTimestamp = Unity.Entities.SystemAPI.Time.ElapsedTime;

        if (0u == targetUnit.ValueRO.health) {
          // ⟶ Structural changes to Unity’s ECS not allowed while enumerating them i.e. `state.EntityManager.DestroyEntity(…)`
          // ⟶ Ensure you do not access an entity that does not exist anymore i.e. similar to `UnityEngine.GameObject`s or nullable's
          commands.DestroyEntity(finder.ValueRO.target);
        }

        else {
          Unity.Entities.Entity                 bulletEntity = state.EntityManager.Instantiate(soldier.ValueRO.bulletPrefabrication);
          Unity.Entities.RefRW<BulletComponent> bullet       = Unity.Entities.SystemAPI.GetComponentRW<BulletComponent>(bulletEntity);

          // …
          Unity.Entities.SystemAPI.SetComponent(bulletEntity, Unity.Transforms.LocalTransform.FromPosition(localTransform.ValueRO.Position));
          bullet.ValueRW.target = finder.ValueRO.target;
        }
      }

      commands.Playback(state.EntityManager);
      state.Dependency = default; // ⟶ `Unity.Jobs.JobHandle.CombineDependencies(state.Dependency, job, …)`
    }
  }
