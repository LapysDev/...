float dot = Mathf.Clamp(Vector3.Dot(a.normalized, b.normalized), -1f, 1f);
float omega = Mathf.Acos(dot);
if (omega < 1e-6f)
    return Vector3.Lerp(a, b, t);  // fallback when angle small
float sinOmega = Mathf.Sin(omega);
return (Mathf.Sin((1 - t) * omega) / sinOmega) * a +
       (Mathf.Sin(t * omega) / sinOmega) * b;
