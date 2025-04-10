public sealed class Unit : UnityEngine.MonoBehaviour {
  public const float TARGET_DISTANCE_SQUARED = 2.0f;

  public float angularSpeed = 15.0f; // ⟶ In degrees°
  public float speed        = 2.5f;
}
  public class UnitBaker : Unity.Entities.Baker<Unit> {
    public override void Bake(Unit unit) {
      this.AddComponent(this.GetEntity(Unity.Entities.TransformUsageFlags.Dynamic), new UnitComponent() {
        angularSpeed   = unit.angularSpeed,
        speed          = unit.speed,
        targetPosition = unit.transform.position
      });
    }
  }

  public struct UnitComponent : Unity.Entities.IComponentData {
    public float                    angularSpeed;
    public float                    speed;
    public Unity.Mathematics.float3 targetPosition;
  }

  [Unity.Burst.BurstCompile]
  public partial struct UnitSystem : Unity.Entities.ISystem {
    [Unity.Burst.BurstCompile]
    private partial struct UnitJob : IJobEntity {
      public float deltaTime;

      /* … */
      public void Execute(in UnitComponent unit, ref Unity.Transforms.LocalTransform localTransform, ref Unity.Physics.PhysicsVelocity physicsVelocity) {
        Unity.Mathematics.float3 moveDirection = unit.targetPosition - localTransform.Position;

        // …
        if (Unit.TARGET_DISTANCE_SQUARED >= Unity.Mathematics.math.lengthsq(moveDirection))
          physicsVelocity.Linear = Unity.Mathematics.float3.zero;

        else {
          moveDirection            = Unity.Mathematics.math.normalize(moveDirection);                                                                                                                                  //
          localTransform .Rotation = Unity.Mathematics.math.slerp(localTransform.Rotation, Unity.Mathematics.quaternion.LookRotation(moveDirection, Unity.Mathematics.math.up()), this.deltaTime * unit.angularSpeed); // ⟶ `Unity.Mathematics.quaternion.LookRotation(moveDirection, Unity.Mathematics.math.up())`
          physicsVelocity.Linear   = moveDirection * unit.speed;                                                                                                                                                       // ⟶ `localTransform.Position += Unity.Entities.SystemAPI.Time.DeltaTime * unit.speed * moveDirection`
        }

        physicsVelocity.Angular = Unity.Mathematics.float3.zero;
      }
    }

    /* … */
    [Unity.Burst.BurstCompile]
    public void OnUpdate(ref Unity.Entities.SystemState state) {
      UnitJob job = new() {deltaTime = Unity.Entities.SystemAPI.Time.DeltaTime};

      // …
      #if true // ⟶ Multi-threaded
        job.ScheduleParallel();
      #else // ⟶ Single-threaded i.e. `job.Run()`
        foreach (var (unit, localTransform, physicsVelocity) in Unity.Entities.SystemAPI.Query<
          Unity.Entities.RefRO<UnitComponent>,
          Unity.Entities.RefRW<Unity.Transforms.LocalTransform>,
          Unity.Entities.RefRW<Unity.Physics.PhysicsVelocity>
        >()) {
          Unity.Mathematics.float3 moveDirection = unit.ValueRO.targetPosition - localTransform.ValueRO.Position;

          // …
          if (UnitComponent.TARGET_DISTANCE_SQUARED >= Unity.Mathematics.math.lengthsq(moveDirection))
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
