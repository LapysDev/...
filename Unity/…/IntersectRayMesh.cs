using UnityEngine;
using static System.Reflection.BindingFlags;

public static class MeshFilterExtensions
{
    public static RaycastHit? IntersectRayMesh(this MeshFilter meshFilter, in Ray ray)
        => intersectRayMeshFunc(ray, meshFilter.sharedMesh, meshFilter.transform.localToWorldMatrix, out var result) ? result : null;

    delegate bool IntersectRayMeshDelegate(Ray ray, Mesh mesh, Matrix4x4 matrix, out RaycastHit hit);

    static readonly IntersectRayMeshDelegate intersectRayMeshFunc
        =
#if UNITY_EDITOR
        (IntersectRayMeshDelegate)
        typeof(UnityEditor.HandleUtility)
            .GetMethod("IntersectRayMesh", Static | NonPublic)
            .CreateDelegate(typeof(IntersectRayMeshDelegate));
#else
            null;
#endif
}
