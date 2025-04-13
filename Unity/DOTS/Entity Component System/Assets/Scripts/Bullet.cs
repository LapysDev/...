public sealed class Bullet : UnityEngine.MonoBehaviour {
  public const float TARGET_REACH_DISTANCE_SQUARED = 0.2f;

  public uint  damage = 10u;
  public float speed  = 65.0f;
}
  public sealed class BulletBaker : Unity.Entities.Baker<Bullet> {
    public override void Bake(Bullet bullet) {
      this.AddComponent(this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic), new BulletComponent() {damage = bullet.damage, speed = bullet.speed, target = Unity.Entities.Entity.Null});
    }
  }

  public struct BulletComponent : Unity.Entities.IComponentData {
    public uint                  damage;
    public float                 speed;
    public Unity.Entities.Entity target;
  }

  [Unity.Burst.BurstCompile]
  [Unity.Entities.UpdateInGroup(typeof(Unity.Entities.SimulationSystemGroup))]
  public partial struct BulletSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      Unity.Entities.EntityCommandBuffer commands = new(Unity.Collections.Allocator.Temp);

      // …
      foreach (var (bullet, localTransform, entity) in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<BulletComponent>, Unity.Entities.RefRW<Unity.Transforms.LocalTransform>>().WithEntityAccess())
      if (Unity.Entities.SystemAPI.Exists(bullet.ValueRO.target) && Unity.Entities.Entity.Null != bullet.ValueRO.target) {
        Unity.Transforms.LocalTransform bulletTargetLocalTransform = Unity.Entities.SystemAPI.GetComponent<Unity.Transforms.LocalTransform>(bullet.ValueRO.target);
        Unity.Mathematics.float3        moveDirection              = bulletTargetLocalTransform.Position - localTransform.ValueRO.Position;

        // …
        localTransform.ValueRW.Position += Unity.Mathematics.math.normalize(moveDirection) * bullet.ValueRO.speed * Unity.Entities.SystemAPI.Time.DeltaTime;

        if (Bullet.TARGET_REACH_DISTANCE_SQUARED >= Unity.Mathematics.math.distancesq(localTransform.ValueRO.Position, bulletTargetLocalTransform.Position)) {
          Unity.Entities.RefRW<UnitComponent> targetUnit = Unity.Entities.SystemAPI.GetComponentRW<UnitComponent>(bullet.ValueRO.target);

          // …
          targetUnit.ValueRW.health = Unity.Mathematics.math.max(0u, (uint) ((int) targetUnit.ValueRO.health - bullet.ValueRO.damage));
          commands.DestroyEntity(entity);
        }
      } else commands.DestroyEntity(entity);

      commands.Playback(state.EntityManager);
    }
  }
