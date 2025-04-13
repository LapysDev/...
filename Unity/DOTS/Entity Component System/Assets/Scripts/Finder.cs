public sealed class Finder : UnityEngine.MonoBehaviour {
  public float     range      = 0.0f; // ⟶ `7.0f` for `Soldier` and `5.0f` for `Zombie` by default
  public Unit.Kind targetKind = Unit.Kind._;
}
  public sealed class FinderBaker : Unity.Entities.Baker<Finder> {
    public override void Bake(Finder finder) {
      this.AddComponent(this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic), new FinderComponent() {range = finder.range, target = Unity.Entities.Entity.Null, targetKind = finder.targetKind});
    }
  }

  public struct FinderComponent : Unity.Entities.IComponentData {
    public float                 range;
    public Unity.Entities.Entity target;
    public Unit.Kind             targetKind;
  }

  [Unity.Burst.BurstCompile]
  [Unity.Entities.UpdateInGroup(typeof(Unity.Entities.SimulationSystemGroup))]
  public partial struct FinderSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      Unity.Physics.CollisionWorld                            collisionWorld             = Unity.Entities.SystemAPI.GetSingleton<Unity.Physics.PhysicsWorldSingleton>().CollisionWorld;
      Unity.Collections.NativeList<Unity.Physics.DistanceHit> collisionWorldDistanceHits = new(Unity.Collections.Allocator.Temp);

      // …
      foreach (var finder in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRW<FinderComponent>>()) {
        if (!Unity.Entities.SystemAPI.Exists(finder.ValueRO.target))
        finder.ValueRW.target = Unity.Entities.Entity.Null;
      }

      foreach (var (finder, localTransform) in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRW<FinderComponent>, Unity.Entities.RefRO<Unity.Transforms.LocalTransform>>()) {
        collisionWorldDistanceHits.Clear();

        if (collisionWorld.OverlapSphere(localTransform.ValueRO.Position, finder.ValueRO.range, ref collisionWorldDistanceHits, new() {BelongsTo = ~0b0000u, CollidesWith = Game.PHYSICS_LAYERS.Default, GroupIndex = 0}))
        foreach (Unity.Physics.DistanceHit collisionWorldDistanceHit in collisionWorldDistanceHits) {
          UnitComponent targetUnit = Unity.Entities.SystemAPI.GetComponent<UnitComponent>(collisionWorldDistanceHit.Entity);

          if (finder.ValueRO.targetKind == targetUnit.kind) {
            finder.ValueRW.target = collisionWorldDistanceHit.Entity;
            break;
          }
        }
      }
    }
  }
