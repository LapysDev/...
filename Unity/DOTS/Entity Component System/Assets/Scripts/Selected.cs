public sealed class Selected : UnityEngine.MonoBehaviour {
  public const float SELECTION_AREA_THRESHOLD_SIZE = 400.0f;

  public float                   scale  = 2.5f;  // ⟶ `0.0f` hides the `.visual`
  public UnityEngine.GameObject? visual = null; // ⟶ Assigned from inspector; Expected to be non-null
}
  public class SelectedBaker : Unity.Entities.Baker<Selected> {
    public override void Bake(Selected selected) {
      Unity.Entities.Entity entity = this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic);

      // …
      this.AddComponent(entity, new SelectedComponent() {
        scale  = selected.scale,
        visual = this.GetEntity(selected.visual!, Unity.Entities.TransformUsageFlags.Dynamic)
      });
      this.SetComponentEnabled<SelectedComponent>(entity, false);
    }
  }

  public struct SelectedComponent : Unity.Entities.IComponentData, Unity.Entities.IEnableableComponent {
    public float                 scale;
    public Unity.Entities.Entity visual;
  }

  [Unity.Burst.BurstCompile]
  public partial struct SelectedSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      foreach (var selected in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<SelectedComponent>>())
      Unity.Entities.SystemAPI.GetComponentRW<Unity.Transforms.LocalTransform>(selected.ValueRO.visual).ValueRW.Scale = selected.ValueRO.scale;

      foreach (var selected in Unity.Entities.SystemAPI.Query<Unity.Entities.RefRO<SelectedComponent>>().WithDisabled<SelectedComponent>())
      Unity.Entities.SystemAPI.GetComponentRW<Unity.Transforms.LocalTransform>(selected.ValueRO.visual).ValueRW.Scale = 0.0f;
    }
  }
