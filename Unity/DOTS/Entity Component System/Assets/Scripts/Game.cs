global using Unity.Entities; // ⟶ Resolve Unity's code generation

/* … */
public static class Game {
  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(GameBehaviour));
}

/* … */
public sealed class GameBehaviour : UnityEngine.MonoBehaviour {
  private static ref Unity.Entities.EntityManager    ENTITY_MANAGER           { get { GameBehaviour.ENTITY_MANAGER_UNCHECKED ??= new[] {Unity.Entities.World.DefaultGameObjectInjectionWorld.EntityManager}; return ref GameBehaviour.ENTITY_MANAGER_UNCHECKED[0]; } }
  private static     Unity.Entities.EntityManager[]? ENTITY_MANAGER_UNCHECKED = null;
  private static ref UnityEngine.Camera              POINTER_CAMERA           { get { GameBehaviour.POINTER_CAMERA_UNCHECKED ??= new[] {UnityEngine.Camera.main}; return ref GameBehaviour.POINTER_CAMERA_UNCHECKED[0]; } }
  private static     UnityEngine.Camera[]?           POINTER_CAMERA_UNCHECKED = null;

  private UnityEngine.Vector2 selectionStartPosition = UnityEngine.Vector2.zero;

  /* … */
  public static void PositionsToArea(UnityEngine.Vector2 selectionStartPosition, UnityEngine.Vector2 selectionEndPosition, out UnityEngine.Rect area) {
    UnityEngine.Vector2 areaLowerLeftCorner  = new(UnityEngine.Mathf.Min(selectionStartPosition.x, selectionEndPosition.x), UnityEngine.Mathf.Min(selectionStartPosition.y, selectionEndPosition.y));
    UnityEngine.Vector2 areaUpperRightCorner = new(UnityEngine.Mathf.Max(selectionStartPosition.x, selectionEndPosition.x), UnityEngine.Mathf.Max(selectionStartPosition.y, selectionEndPosition.y));

    area = new(areaLowerLeftCorner.x, areaLowerLeftCorner.y, areaUpperRightCorner.x - areaLowerLeftCorner.x, areaUpperRightCorner.y - areaLowerLeftCorner.y);
  }

  // [Unity.Burst.BurstDiscard] // ⟶ Reminder: Do not use managed features in `Unity.Burst.BurstCompile` structures; Otherwise `return void`
  public static void ScreenToWorldPosition(UnityEngine.Vector2 screenPosition, out UnityEngine.Vector3 worldPosition) {
    UnityEngine.Ray pointerCameraRay = GameBehaviour.POINTER_CAMERA.ScreenPointToRay(screenPosition);
    worldPosition = new UnityEngine.Plane(UnityEngine.Vector3.up, UnityEngine.Vector3.zero).Raycast(pointerCameraRay, out float distance) ? pointerCameraRay.GetPoint(distance) : UnityEngine.Vector3.zero;
  }

  private void OnDestroy() {
    GameBehaviour.ENTITY_MANAGER_UNCHECKED = null;
  }

  private void Update() {
    UnityEngine.Vector2 pointerPosition = UnityEngine.Input.mousePosition;

    // … ⟶ Toggle all `SelectedComponent`s in an area on left mouse click and release
    if (UnityEngine.Input.GetMouseButtonDown(0))
    this.selectionStartPosition = pointerPosition;

    if (UnityEngine.Input.GetMouseButtonUp(0)) {
      Unity.Entities.EntityQuery entityDeselectionQuery = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<SelectedComponent>                             ()                                 .Build(GameBehaviour.ENTITY_MANAGER);

      // …
      GameBehaviour.PositionsToArea(this.selectionStartPosition, pointerPosition, out UnityEngine.Rect selectionArea);

      foreach (Unity.Entities.Entity entity in entityDeselectionQuery.ToEntityArray(Unity.Collections.Allocator.Temp))
      GameBehaviour.ENTITY_MANAGER.SetComponentEnabled<SelectedComponent>(entity, false);

      // … ⟶ Single-selection or multi-selection
      if (Selected.SELECTION_AREA_THRESHOLD_SIZE >= selectionArea.height * selectionArea.width) {
        Unity.Entities.EntityQuery                             entitySelectionQuery       = GameBehaviour.ENTITY_MANAGER.CreateEntityQuery(typeof(Unity.Physics.PhysicsWorldSingleton));
        UnityEngine.Ray                                        pointerRay                 = GameBehaviour.POINTER_CAMERA.ScreenPointToRay(pointerPosition); // ⟶ Not to be confused with `Unity.Physics.Ray`
        Unity.Physics.PhysicsWorldSingleton                    physicsWorld               = entitySelectionQuery.GetSingleton<Unity.Physics.PhysicsWorldSingleton>();
        Unity.Physics.CollisionWorld                           physicsWorldCollisionWorld = physicsWorld.CollisionWorld;
        Unity.Collections.NativeList<Unity.Physics.RaycastHit> physicsWorldRaycastHits    = new(Unity.Collections.Allocator.Temp);
        Unity.Physics.RaycastInput                             physicsWorldRaycastInput   = new() {End = pointerRay.GetPoint(1.0e5f), Filter = new() {BelongsTo = ~0b0000u, CollidesWith = (uint) UnityEngine.LayerMask.GetMask("Default"), GroupIndex = 0}, Start = pointerRay.GetPoint(0.0f)};

        // …
        if (physicsWorldCollisionWorld.CastRay(physicsWorldRaycastInput, out Unity.Physics.RaycastHit physicsWorldRaycastHit)) {
          if (GameBehaviour.ENTITY_MANAGER.HasComponent<UnitComponent>(physicsWorldRaycastHit.Entity))
          GameBehaviour.ENTITY_MANAGER.SetComponentEnabled<SelectedComponent>(physicsWorldRaycastHit.Entity, true);
        }
      }

      else {
        Unity.Entities.EntityQuery                                     entitySelectionQuery   = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<Unity.Transforms.LocalTransform, UnitComponent>().WithPresent<SelectedComponent>().Build(GameBehaviour.ENTITY_MANAGER);
        Unity.Collections.NativeArray<Unity.Entities.Entity>           entities               = entitySelectionQuery.ToEntityArray                                        (Unity.Collections.Allocator.Temp);
        Unity.Collections.NativeArray<Unity.Transforms.LocalTransform> localTransforms        = entitySelectionQuery.ToComponentDataArray<Unity.Transforms.LocalTransform>(Unity.Collections.Allocator.Temp);

        // …
        for (int index = localTransforms.Length; 0 != index--; ) {
          if (selectionArea.Contains(GameBehaviour.POINTER_CAMERA.WorldToScreenPoint(localTransforms[index].Position)))
          GameBehaviour.ENTITY_MANAGER.SetComponentEnabled<SelectedComponent>(entities[index], true);
        }
      }
    }

    // … ⟶ Update all `UnitComponent.targetPosition`s on right mouse click
    if (UnityEngine.Input.GetMouseButtonDown(1)) {
      Unity.Entities.EntityQuery                           entityQuery = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<SelectedComponent, UnitComponent>().Build(GameBehaviour.ENTITY_MANAGER);
      Unity.Collections.NativeArray<Unity.Entities.Entity> entities    = entityQuery.ToEntityArray                      (Unity.Collections.Allocator.Temp);
      Unity.Collections.NativeArray<UnitComponent>         units       = entityQuery.ToComponentDataArray<UnitComponent>(Unity.Collections.Allocator.Temp);

      // …
      GameBehaviour.ScreenToWorldPosition(pointerPosition, out UnityEngine.Vector3 pointerWorldPosition);

      for (int index = units.Length; 0 != index--; ) {
        UnitComponent unit = units[index];

        unit.targetPosition = pointerWorldPosition;
        units[index]        = unit; // ⟶ `GameBehaviour.ENTITY_MANAGER.SetComponentData(entities[index], unit)`
      }

      entityQuery.CopyFromComponentDataArray(units);
    }
  }
}
