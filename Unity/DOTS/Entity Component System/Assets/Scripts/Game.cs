global using Unity.Entities; // ⟶ Resolve Unity's code generation

/* … */
internal static class Game {
  public   static ref      UnityEngine.Camera                                                                  POINTER_CAMERA                 { get { Game.POINTER_CAMERA_UNCHECKED ??= new[] {UnityEngine.Camera.main}; return ref Game.POINTER_CAMERA_UNCHECKED[0]; } }
  internal static          UnityEngine.Camera[]?                                                               POINTER_CAMERA_UNCHECKED       = null;
  public   static readonly (uint _, uint Default, uint IgnoreRaycast, uint TransparentFX, uint UI, uint Water) PHYSICS_LAYERS                 = (default, 1u << 0, 1u << 2, 1u << 1, 1u << 5, 1u << 4); // ⟶ `(default, UnityEngine.LayerMask.GetMask("Default"), …)`
  public   static ref      Unity.Entities.World                                                                WORLD                          { get { Game.WORLD_UNCHECKED ??= new[] {Unity.Entities.World.DefaultGameObjectInjectionWorld}; return ref Game.WORLD_UNCHECKED[0]; } }
  internal static          Unity.Entities.World[]?                                                             WORLD_UNCHECKED                = null;
  public   static ref      Unity.Entities.EntityManager                                                        WORLD_ENTITY_MANAGER           { get { Game.WORLD_ENTITY_MANAGER_UNCHECKED ??= new[] {Game.WORLD.EntityManager}; return ref Game.WORLD_ENTITY_MANAGER_UNCHECKED[0]; } }
  internal static          Unity.Entities.EntityManager[]?                                                     WORLD_ENTITY_MANAGER_UNCHECKED = null;

  /* … */
  internal static void GetPositionsFromTargetPosition(Unity.Mathematics.float3 targetPosition, uint count, float radius, out Unity.Collections.NativeArray<Unity.Mathematics.float3> positions) {
    positions = new((int) count, Unity.Collections.Allocator.Temp);
    if (0u == count) return;

    positions[0] = targetPosition;
    if (count == 1u) return;

    for ((int index, uint layer) = (1, 0u); count != index; ++layer) {
      for ((uint subindex, uint positionCount) = (0u, (layer * 2u) + 3u); count != index && positionCount != subindex; ++subindex)
      positions[index++] = targetPosition + Unity.Mathematics.math.rotate(
        Unity.Mathematics.quaternion.RotateY(subindex * (Unity.Mathematics.math.PI2 / positionCount)),
        new Unity.Mathematics.float3(radius * (layer + 1u), 0.0f, 0.0f)
      );
    }
  }

  [UnityEngine.RuntimeInitializeOnLoadMethod]
  private static void Main() => new UnityEngine.GameObject("…", typeof(GameBehaviour));

  internal static void PositionsToArea(UnityEngine.Vector2 selectionStartPosition, UnityEngine.Vector2 selectionEndPosition, out UnityEngine.Rect area) {
    UnityEngine.Vector2 areaLowerLeftCorner  = new(UnityEngine.Mathf.Min(selectionStartPosition.x, selectionEndPosition.x), UnityEngine.Mathf.Min(selectionStartPosition.y, selectionEndPosition.y));
    UnityEngine.Vector2 areaUpperRightCorner = new(UnityEngine.Mathf.Max(selectionStartPosition.x, selectionEndPosition.x), UnityEngine.Mathf.Max(selectionStartPosition.y, selectionEndPosition.y));

    area = new(areaLowerLeftCorner.x, areaLowerLeftCorner.y, areaUpperRightCorner.x - areaLowerLeftCorner.x, areaUpperRightCorner.y - areaLowerLeftCorner.y);
  }

  // [Unity.Burst.BurstDiscard] // ⟶ Reminder: Do not use managed features in `Unity.Burst.BurstCompile` structures; Otherwise `return void`
  internal static void ScreenToWorldPosition(UnityEngine.Vector2 screenPosition, out UnityEngine.Vector3 worldPosition) {
    UnityEngine.Ray pointerCameraRay = Game.POINTER_CAMERA.ScreenPointToRay(screenPosition);
    worldPosition = new UnityEngine.Plane(UnityEngine.Vector3.up, UnityEngine.Vector3.zero).Raycast(pointerCameraRay, out float distance) ? pointerCameraRay.GetPoint(distance) : UnityEngine.Vector3.zero;
  }
}

/* … */
internal sealed class GameBehaviour : UnityEngine.MonoBehaviour {
  private UnityEngine.Vector2 selectionStartPosition = UnityEngine.Vector2.zero;

  /* … */
  private void OnDestroy() {
    Game.WORLD_ENTITY_MANAGER_UNCHECKED = null;
    Game.WORLD_UNCHECKED          = null;
  }

  private void Update() {
    UnityEngine.Vector2 pointerPosition = UnityEngine.Input.mousePosition;

    // … ⟶ Toggle all `SelectableComponent`s in an area on left mouse click and release
    if (UnityEngine.Input.GetMouseButtonDown(0))
    this.selectionStartPosition = pointerPosition;

    if (UnityEngine.Input.GetMouseButtonUp(0)) {
      Unity.Entities.EntityQuery entityDeselectionQuery = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<SelectableComponent>().Build(Game.WORLD_ENTITY_MANAGER);

      // …
      Game.PositionsToArea(this.selectionStartPosition, pointerPosition, out UnityEngine.Rect selectionArea);

      foreach (Unity.Entities.Entity entity in entityDeselectionQuery.ToEntityArray(Unity.Collections.Allocator.Temp))
      Game.WORLD_ENTITY_MANAGER.SetComponentEnabled<SelectableComponent>(entity, false);

      // … ⟶ Single-selection or multi-selection
      if (Selectable.SELECTION_AREA_THRESHOLD_SIZE >= selectionArea.height * selectionArea.width) {
        // ⟶ See shorter version of this in `FindableSystem.OnUpdate(…)` function
        Unity.Entities.EntityQuery                             entitySelectionQuery       = Game.WORLD_ENTITY_MANAGER.CreateEntityQuery(typeof(Unity.Physics.PhysicsWorldSingleton)); // ⟶ Same as what `EntityQueryBuilder..Build(…)` does
        UnityEngine.Ray                                        pointerRay                 = Game.POINTER_CAMERA.ScreenPointToRay(pointerPosition); // ⟶ Not to be confused with `Unity.Physics.Ray`
        Unity.Physics.PhysicsWorldSingleton                    physicsWorld               = entitySelectionQuery.GetSingleton<Unity.Physics.PhysicsWorldSingleton>();
        Unity.Physics.CollisionWorld                           physicsWorldCollisionWorld = physicsWorld.CollisionWorld;
        Unity.Collections.NativeList<Unity.Physics.RaycastHit> physicsWorldRaycastHits    = new(Unity.Collections.Allocator.Temp);
        Unity.Physics.RaycastInput                             physicsWorldRaycastInput   = new() {End = pointerRay.GetPoint(1.0e5f), Filter = new() {BelongsTo = ~0b0000u, CollidesWith = Game.PHYSICS_LAYERS.Default, GroupIndex = 0}, Start = pointerRay.GetPoint(0.0f)};

        // …
        if (physicsWorldCollisionWorld.CastRay(physicsWorldRaycastInput, out Unity.Physics.RaycastHit physicsWorldRaycastHit)) {
          if (Game.WORLD_ENTITY_MANAGER.HasComponent<SelectableComponent>(physicsWorldRaycastHit.Entity))
          Game.WORLD_ENTITY_MANAGER.SetComponentEnabled<SelectableComponent>(physicsWorldRaycastHit.Entity, true);
        }
      }

      else {
        Unity.Entities.EntityQuery                                     entitySelectionQuery = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<Unity.Transforms.LocalTransform, UnitComponent>().WithPresent<SelectableComponent>().Build(Game.WORLD_ENTITY_MANAGER);
        Unity.Collections.NativeArray<Unity.Entities.Entity>           entities             = entitySelectionQuery.ToEntityArray                                        (Unity.Collections.Allocator.Temp);
        Unity.Collections.NativeArray<Unity.Transforms.LocalTransform> localTransforms      = entitySelectionQuery.ToComponentDataArray<Unity.Transforms.LocalTransform>(Unity.Collections.Allocator.Temp);

        // …
        for (int index = localTransforms.Length; 0 != index--; ) {
          if (selectionArea.Contains(Game.POINTER_CAMERA.WorldToScreenPoint(localTransforms[index].Position)))
          Game.WORLD_ENTITY_MANAGER.SetComponentEnabled<SelectableComponent>(entities[index], true);
        }
      }
    }

    // … ⟶ Update all `UnitComponent.targetPosition`s on right mouse click
    if (UnityEngine.Input.GetMouseButtonDown(1)) {
      Unity.Entities.EntityQuery                           entityQuery     = new Unity.Entities.EntityQueryBuilder(Unity.Collections.Allocator.Temp).WithAll<SelectableComponent, UnitComponent>().Build(Game.WORLD_ENTITY_MANAGER);
      Unity.Collections.NativeArray<Unity.Entities.Entity> entities        = entityQuery.ToEntityArray                      (Unity.Collections.Allocator.Temp);
      Unity.Collections.NativeArray<UnitComponent>         selectableUnits = entityQuery.ToComponentDataArray<UnitComponent>(Unity.Collections.Allocator.Temp);

      // …
      Game.ScreenToWorldPosition         (pointerPosition, out UnityEngine.Vector3 pointerWorldPosition);
      Game.GetPositionsFromTargetPosition(pointerWorldPosition, (uint) entityQuery.CalculateEntityCount() /* ⟶ not `.IsEmpty` */, Unit.TARGET_GROUP_DISTANCE_SQUARED, out Unity.Collections.NativeArray<Unity.Mathematics.float3> targetPositions);

      for (int index = selectableUnits.Length; 0 != index--; ) {
        UnitComponent selectableUnit = selectableUnits[index];

        selectableUnit.targetPosition = targetPositions[index];
        selectableUnits[index]        = selectableUnit; // ⟶ `Game.WORLD_ENTITY_MANAGER.SetComponentData(entities[index], unit)`
      }

      entityQuery.CopyFromComponentDataArray(selectableUnits);
    }
  }
}
