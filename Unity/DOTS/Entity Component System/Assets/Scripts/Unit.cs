public sealed class Unit : UnityEngine.MonoBehaviour {
  public enum Kind : byte { _, Soldier, Zombie }

  public partial struct SoldierComponent : Unity.Entities.IComponentData {
    public double reloadInterval;
    public double reloadTimestamp;
  }

  public partial struct ZombieComponent : Unity.Entities.IComponentData {}

  /* … */
  public const uint  HEALTH_MAXIMUM                = 200u;
  public const uint  SOLDIER_DAMAGE_MAXIMUM        = 10u;
  public const uint  SOLDIER_HEALTH_MAXIMUM        = 120u;
  public const float TARGET_GROUP_DISTANCE_SQUARED = 4.0f;
  public const float TARGET_REACH_DISTANCE_SQUARED = 2.0f;
  public const uint  ZOMBIE_DAMAGE_MAXIMUM         = 20u;
  public const uint  ZOMBIE_HEALTH_MAXIMUM         = 70u;

  public                                             float     angularSpeed   = 15.0f; // ⟶ In degrees°
  public                                             uint      damage         = 0u;
  [UnityEngine.Range(0, Unit.HEALTH_MAXIMUM)] public uint      health         = Unit.HEALTH_MAXIMUM;
  public                                             Unit.Kind kind           = Unit.Kind._;
  public                                             double    reloadInterval = 0.2; // ⟶ Reload ammo every ⅕ seconds
  public                                             float     speed          = 2.5f;

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
        case Unit.Kind.Soldier: this.AddComponent(entity, new Unit.SoldierComponent() {reloadInterval = unit.reloadInterval, reloadTimestamp = 0.0}); break;
        case Unit.Kind.Zombie : this.AddComponent(entity, new Unit.ZombieComponent ()); break;
        default: break;
      }
    }
  }

  public struct UnitComponent : Unity.Entities.IComponentData {
    public float                    angularSpeed;
    public uint                     damage;
    public uint                     health;
    public Unit.Kind                kind;
    public float                    speed;
    public Unity.Mathematics.float3 targetPosition;
  }

  [Unity.Burst.BurstCompile]
  // [Unity.Entities.UpdateBefore(typeof(SelectedSystem))]
  public partial struct UnitSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    private partial struct UnitJob : IJobEntity {
      internal partial struct SoldierJob : IJobEntity {
        public double elapsedTime;

        /* … */
        public void Execute(in FinderComponent finder, ref Unit.SoldierComponent soldier) {
          if (Unity.Entities.Entity.Null != finder.targetEntity)
          if (soldier.reloadInterval <= this.elapsedTime - soldier.reloadTimestamp) {
            soldier.reloadTimestamp = this.elapsedTime;

            // …
          }
        }
      }

      /* … */
      public float deltaTime;

      /* … */
      public void Execute(in UnitComponent unit, ref Unity.Transforms.LocalTransform localTransform, ref Unity.Physics.PhysicsVelocity physicsVelocity) {
        Unity.Mathematics.float3 moveDirection = unit.targetPosition - localTransform.Position;

        // …
        //
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
      #if true // ⟶ Multi-threaded
        new UnitJob.SoldierJob() {elapsedTime = Unity.Entities.SystemAPI.Time.ElapsedTime}.ScheduleParallel();
        new UnitJob           () {deltaTime   = Unity.Entities.SystemAPI.Time.DeltaTime}  .ScheduleParallel();
      #else // ⟶ Single-threaded i.e. `Unity.Entities.IJobEntity::Run()`
        foreach (var (unit, localTransform, physicsVelocity) in Unity.Entities.SystemAPI.Query<
          Unity.Entities.RefRO<UnitComponent>,
          Unity.Entities.RefRW<Unity.Transforms.LocalTransform>,
          Unity.Entities.RefRW<Unity.Physics.PhysicsVelocity>
        >()) {
          Unity.Mathematics.float3 moveDirection = unit.ValueRO.targetPosition - localTransform.ValueRO.Position;

          // …
          if (UnitComponent.TARGET_REACH_DISTANCE_SQUARED >= Unity.Mathematics.math.lengthsq(moveDirection))
            physicsVelocity.ValueRW.Linear = Unity.Mathematics.float3.zero;

          else {
            moveDirection            = Unity.Mathematics.math.normalize(moveDirection);                                                                                                                                                                                   //
            localTransform .ValueRW.Rotation = Unity.Mathematics.math.slerp(localTransform.ValueRO.Rotation, Unity.Mathematics.quaternion.LookRotation(moveDirection, Unity.Mathematics.math.up()), Unity.Entities.SystemAPI.Time.DeltaTime * unit.ValueRO.angularSpeed); // ⟶ `Unity.Mathematics.quaternion.LookRotation(moveDirection, Unity.Mathematics.math.up())`
            physicsVelocity.ValueRW.Linear   = moveDirection * unit.ValueRO.speed;                                                                                                                                                                                        // ⟶ `localTransform.ValueRW.Position += Unity.Entities.SystemAPI.Time.DeltaTime * unit.ValueRO.speed * moveDirection`
          }

          physicsVelocity.ValueRW.Angular = Unity.Mathematics.float3.zero;
        }
      #endif
    }
  }
