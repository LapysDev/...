public sealed class Selectable : UnityEngine.MonoBehaviour {
  public const float SELECTION_AREA_THRESHOLD_SIZE = 400.0f;

  public float                   scale  = 2.5f;  // ⟶ `0.0f` hides the `.visual`
  public UnityEngine.GameObject? visual = null; // ⟶ Assigned from inspector; Expected to be non-null
}
  public sealed class SelectableBaker : Unity.Entities.Baker<Selectable> {
    public override void Bake(Selectable selectable) {
      Unity.Entities.Entity entity = this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic);

      // …
      this.AddComponent(entity, new SelectableComponent() {scale = selectable.scale, visual = this.GetEntity(selectable.visual!, Unity.Entities.TransformUsageFlags.Dynamic)});
      this.SetComponentEnabled<SelectableComponent>(entity, false);
    }
  }

  public struct SelectableComponent : Unity.Entities.IComponentData, Unity.Entities.IEnableableComponent {
    public float                 scale;
    public Unity.Entities.Entity visual;
  }

  [Unity.Burst.BurstCompile]
  [Unity.Entities.UpdateInGroup(typeof(Unity.Entities.SimulationSystemGroup))]
  public partial struct SelectableSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      foreach (var selectable in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<SelectableComponent>>())
      Unity.Entities.SystemAPI.GetComponentRW<Unity.Transforms.LocalTransform>(selectable.ValueRO.visual).ValueRW.Scale = selectable.ValueRO.scale;

      foreach (var selectable in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<SelectableComponent>>().WithDisabled<SelectableComponent>())
      Unity.Entities.SystemAPI.GetComponentRW<Unity.Transforms.LocalTransform>(selectable.ValueRO.visual).ValueRW.Scale = 0.0f;
    }
  }
