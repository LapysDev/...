public delegate void TransformHierarchyCallback(TransformHierarchyObservation observation, UnityEngine.Transform transform);

public class TransformHierarchyNotifier : UnityEngine.MonoBehaviour {
  private int childCount = 0;

  /* … */
  protected void Awake() => this.childCount = this.transform.childCount;

  private void ObserveTransformChange(TransformHierarchyObservation nonobservation) {
    UnityEngine.Transform         transform   = this.transform;
    int                           delta       = transform.childCount - this.childCount;
    // TransformHierarchyObservation observation = delta switch { -1 => TransformHierarchyObservation.TransformRemoved, +1 => TransformHierarchyObservation.TransformAdded, _ => nonobservation };
    TransformHierarchyObservation observation = TransformHierarchyObservation.TransformChanged;
    TransformHierarchyObserver?   observer    = null;

    // …
    this.childCount = transform.childCount;

    while (true) {
      observer = transform.GetComponent<TransformHierarchyObserver>();

      if (observer is not null)
      break;

      transform = transform.parent;
    }

    transform = this.transform;

    for (; observer is not null; observer = observer.parent)
    observer.TransformChildChanged(observation, transform);
  }

  private void OnTransformChildrenChanged() => this.ObserveTransformChange(TransformHierarchyObservation.TransformSwapped);

  private void Update() {
    // WARN (Lapys) -> Possible an Editor change could simultaneously add and remove (swap) `UnityEngine.Transform` children within the same frame which bypasses this check and de-syncs some `TransformHierarchyObserver`
    if (this.childCount != this.transform.childCount) {
      UnityEngine.Debug.Log($"“{this.transform.name}” updated");
      this.ObserveTransformChange(TransformHierarchyObservation.TransformChanged);
    }
  }
}

public enum TransformHierarchyObservation : byte { TransformAdded, TransformChanged, TransformRemoved, TransformSwapped };

public class TransformHierarchyObserver : TransformHierarchyNotifier {
  private  System.Collections.Generic.List<(UnityEngine.Transform transform, int childCount)> hierarchy = null!;
  internal TransformHierarchyObserver?                                                        parent    = null;

  /* … */
  private new void Awake() {
    base.Awake();
    this.hierarchy = TransformHierarchyObserver.ObserveHierarchy(this.transform);
  }

  private static System.Collections.Generic.List<(UnityEngine.Transform, int)> ObserveHierarchy(UnityEngine.Transform transform, bool rooted = true) {
    System.Collections.Generic.Stack<System.Collections.IEnumerator>                   enumerators = new(1);
    System.Collections.Generic.List<(UnityEngine.Transform transform, int childCount)> hierarchy   = new(transform.hierarchyCount);

    // …
    enumerators.Push(transform.GetEnumerator());
    hierarchy.Add((transform, transform.childCount));

    while (0 != enumerators.Count) {
      System.Collections.IEnumerator enumerator = enumerators.Peek();

      // …
      if (enumerator.MoveNext()) {
        transform = (UnityEngine.Transform) enumerator.Current;

        enumerators.Push(transform.GetEnumerator());
        hierarchy.Add((transform, transform.childCount));

        if (transform.GetComponent<TransformHierarchyNotifier>() is null)
        transform.gameObject.AddComponent<TransformHierarchyNotifier>();
      } else enumerators.Pop();
    }

    UnityEngine.Debug.Log($"{(rooted ? "Observed" : "  Subobserved")} {string.Join(", ", hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
    return hierarchy;
  }

  internal void TransformChildChanged(TransformHierarchyObservation observation, UnityEngine.Transform transform) {
    for ((int index, int length) = (0, this.hierarchy.Count); index != length; ++index)
    if (transform == this.hierarchy[index].transform) {
      UnityEngine.Transform?          child      = null;
      int                             childIndex = index;
      int                             count      = this.hierarchy[index].childCount;
      System.Collections.IEnumerator? enumerator = null;
      var                             children   = new {
        added   = new System.Collections.Generic.List<UnityEngine.Transform>(),
        removed = new System.Collections.Generic.List<(UnityEngine.Transform transform, int hierarchyIndex)>(),
        swapped = new System.Collections.Generic.List<((UnityEngine.Transform transform, int hierarchyIndex), (UnityEngine.Transform transform, int hierarchyIndex))>()
      };

      // …
      switch (System.Math.Abs(count - transform.childCount) > 1 ? TransformHierarchyObservation.TransformChanged : observation) {
        case TransformHierarchyObservation.TransformAdded: {
          enumerator = transform.GetEnumerator();

          while (true) {
            if (!enumerator.MoveNext())
              goto case TransformHierarchyObservation.TransformChanged;

            child = (UnityEngine.Transform) enumerator.Current;
            childIndex++;

            if (childIndex == length || child != this.hierarchy[childIndex].transform)
            break;

            UnityEngine.Debug.Log($"Finding added “{this.hierarchy[childIndex].transform.name}”");
            for (uint subcount = (uint) this.hierarchy[childIndex].childCount; 0u != subcount--; ) {
              UnityEngine.Debug.Log($"  Skip “{this.hierarchy[childIndex + 1].transform.name}” ({this.hierarchy[childIndex + 1].childCount})");
              subcount += (uint) this.hierarchy[++childIndex].childCount;
            }
          }

          this.hierarchy[index] = (this.hierarchy[index].transform, count + 1);

          children.added.Add(child);
          this.hierarchy.InsertRange(childIndex, TransformHierarchyObserver.ObserveHierarchy(child.transform, rooted: false));

          UnityEngine.Debug.Log($"“{transform.name}” added “{child.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
        } break;

        case TransformHierarchyObservation.TransformChanged: /* ->> Catch-all solution (including Editor changes) but slower than the other cases */ {
          int CountHierarchy(int index) {
            int count    = this.hierarchy[index].childCount;
            int subcount = count;
            int subindex = 1;

            // …
            while (0 != count--) {
              while (count != subcount)
              subcount += this.hierarchy[index + subindex++].childCount - 1;
            }

            return subindex;
          }

          /* … */
          System.Collections.Generic.List<UnityEngine.Transform>                                 nulls             = new(System.Math.Max(0, transform.childCount - count));
          System.Collections.Generic.List<(int, int)>                                            swaps             = new();
          System.Collections.Generic.List<(UnityEngine.Transform transform, int hierarchyIndex)> transformChildren = new(count);

          // …
          UnityEngine.Debug.Log($"");

          children.removed.Capacity = count;
          enumerator                = transform.GetEnumerator();

          for (int subcount = count; 0u != count--; ) {
            transformChildren.Add((this.hierarchy[childIndex + 1].transform, childIndex + 1));
            UnityEngine.Debug.Log($"“{transform.name}” has child “{this.hierarchy[childIndex + 1].transform.name}” ({this.hierarchy[childIndex + 1].childCount}) @ {childIndex + 1}");

            while (count != subcount)
            subcount += this.hierarchy[++childIndex].childCount - 1;
          }

          children.removed.AddRange(transformChildren);

          for ((count, int enumeratorIndex) = (this.hierarchy[index].childCount, 0); enumerator.MoveNext(); ++enumeratorIndex) {
            bool isContained = false;

            // …
            child = (UnityEngine.Transform) enumerator.Current;

            for (int subindex = 0; count != subindex; ++subindex) {
              UnityEngine.Debug.Log($"#1 => {enumeratorIndex} {subindex} into {count} {transformChildren.Count}");

              if (child == transformChildren[subindex].transform) {
                 UnityEngine.Debug.Log($"#2A");
                isContained = true;

                if (enumeratorIndex != subindex) {
                  UnityEngine.Debug.Log($"#2B");
                  for (; count <= enumeratorIndex; ++count) {
                    UnityEngine.Debug.Log($"#2C");
                    nulls.Add(new UnityEngine.GameObject("NULL #" + nulls.Count).transform);
                    this.hierarchy.Add((nulls[nulls.Count - 1], 0));
                    transformChildren.Add((this.hierarchy[this.hierarchy.Count - 1].transform, this.hierarchy.Count - 1));
                  }

                  children.swapped.Add((
                    (transformChildren[enumeratorIndex].transform, transformChildren[enumeratorIndex].hierarchyIndex),
                    (child,                                        transformChildren[subindex]       .hierarchyIndex)
                  ));
                }

                UnityEngine.Debug.Log($"#2D");
                break;
              } else UnityEngine.Debug.Log($"#2E");
            }

            UnityEngine.Debug.Log($"  “{transform.name}” ({count}) indexed “{child.name}” (was: {isContained})");

            if (!isContained) children.added.Add(child);
            else {
              UnityEngine.Debug.Log($"#3");
              for (int subindex = children.removed.Count; 0 != subindex--; ) {
                if (child == children.removed[subindex].transform) {
                  children.removed.RemoveAt(subindex);
                  break;
                }
              }
              UnityEngine.Debug.Log($"#4");
            }
          }

          foreach ((UnityEngine.Transform subtransform, int hierarchyIndex) in children.removed) {
            for (int subindex = 0; subindex != children.swapped.Count; ++subindex) {
              UnityEngine.Debug.Log($"#5");
              if (children.swapped[subindex].Item1.transform == subtransform || children.swapped[subindex].Item2.transform == subtransform) {
                // UnityEngine.Debug.Log($"WHY? {subindex} of {children.swapped.Count}; {count} {transformChildren.IndexOf((subtransform, hierarchyIndex))}");
                children.swapped.RemoveRange(subindex, count - transformChildren.IndexOf((subtransform, hierarchyIndex)) - 1);
                break;
              }
              UnityEngine.Debug.Log($"#6");
            }
          }

          UnityEngine.Debug.Log($"“{transform.name}” adding {  string.Join(", ", System.Array.ConvertAll(children.added  .ToArray(), static a => $"“{a.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” removing {string.Join(", ", System.Array.ConvertAll(children.removed.ToArray(), static r => $"“{r.transform.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” swapping {string.Join(", ", System.Array.ConvertAll(children.swapped.ToArray(), static s => $"“{s.Item1.transform.name}” with “{s.Item2.transform.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” changing; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");

          children.swapped.Reverse();

          for (int subindex = children.swapped.Count; 0 != subindex--; ) {
            int count1, index1 = children.swapped[subindex].Item1.hierarchyIndex;
            int count2, index2 = children.swapped[subindex].Item2.hierarchyIndex;
            System.Collections.Generic.List<(UnityEngine.Transform, int)> hierarchy1;
            System.Collections.Generic.List<(UnityEngine.Transform, int)> hierarchy2;

            // …
            if (index1 < index2)
            (index1, index2) = (index2, index1);

            if (swaps.Contains((index1, index2))) {
              children.swapped.RemoveAt(subindex);
              continue;
            }

            for (int subsubindex = subindex; 0 != subsubindex--; ) {
              if (children.swapped[subsubindex].Item1.hierarchyIndex == index1) { children.swapped[subsubindex] = ((children.swapped[subsubindex].Item1.transform, hierarchyIndex: index2), children.swapped[subsubindex].Item2); continue; }
              if (children.swapped[subsubindex].Item1.hierarchyIndex == index2) { children.swapped[subsubindex] = ((children.swapped[subsubindex].Item1.transform, hierarchyIndex: index1), children.swapped[subsubindex].Item2); continue; }
              if (children.swapped[subsubindex].Item2.hierarchyIndex == index1) { children.swapped[subsubindex] = (children.swapped[subsubindex].Item1, (children.swapped[subsubindex].Item2.transform, hierarchyIndex: index2)); continue; }
              if (children.swapped[subsubindex].Item2.hierarchyIndex == index2) { children.swapped[subsubindex] = (children.swapped[subsubindex].Item1, (children.swapped[subsubindex].Item2.transform, hierarchyIndex: index1)); continue; }
            }

            for (int subsubindex = children.removed.Count; 0 != subsubindex--; ) {
              if (children.removed[subsubindex].hierarchyIndex == index1) { children.removed[subsubindex] = (children.removed[subsubindex].transform, hierarchyIndex: index2); continue; }
              if (children.removed[subsubindex].hierarchyIndex == index2) { children.removed[subsubindex] = (children.removed[subsubindex].transform, hierarchyIndex: index1); continue; }
            }

            count1     = CountHierarchy(index1);
            count2     = CountHierarchy(index2);
            hierarchy1 = this.hierarchy.GetRange(index1, count1);
            hierarchy2 = this.hierarchy.GetRange(index2, count2);

            swaps.Add((index1, index2));
            this.hierarchy.RemoveRange(index1, count1);
            this.hierarchy.InsertRange(index1, hierarchy2);
            this.hierarchy.RemoveRange(index2, count2);
            this.hierarchy.InsertRange(index2, hierarchy1);

            UnityEngine.Debug.Log($"“{transform.name}” swapped “{children.swapped[subindex].Item1.transform.name}” with “{children.swapped[subindex].Item2.transform.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");

            if (children.removed.Contains(children.swapped[subindex].Item1))
            children.swapped.RemoveAt(subindex);
          }

          children.swapped.Reverse();

          for (int subindex = children.removed.Count; 0 != subindex--; ) {
            this.hierarchy.RemoveRange(children.removed[subindex].hierarchyIndex, CountHierarchy(children.removed[subindex].hierarchyIndex));
            this.hierarchy[index] = (this.hierarchy[index].transform, this.hierarchy[index].childCount - 1);

            UnityEngine.Debug.Log($"“{transform.name}” removed “{children.removed[subindex].transform.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
          }

          childIndex = index + CountHierarchy(index);

          foreach (UnityEngine.Transform subtransform in children.added) {
            System.Collections.Generic.List<(UnityEngine.Transform, int)> hierarchy = TransformHierarchyObserver.ObserveHierarchy(subtransform, rooted: false);
            bool nulled = false;

            // …
            childIndex += hierarchy.Count;
            this.hierarchy[index] = (this.hierarchy[index].transform, this.hierarchy[index].childCount + 1);

            for (int subindex = 0; subindex != this.hierarchy.Count; ++subindex)
            if (nulls.Contains(this.hierarchy[subindex].transform)) {
              nulled = true;
              this.hierarchy.RemoveAt(subindex);
              this.hierarchy.InsertRange(subindex, hierarchy);

              UnityEngine.Debug.Log($"“{transform.name}” added* “{subtransform.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
              break;
            }

            if (nulled)
            continue;

            this.hierarchy.InsertRange(childIndex, hierarchy);
            UnityEngine.Debug.Log($"“{transform.name}” added “{subtransform.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
          }

          foreach (UnityEngine.Transform nool in nulls) {
            UnityEngine.Debug.Log($"  Destroying “{nool.name}”");
            UnityEngine.Object.Destroy(nool.gameObject);
          }

          UnityEngine.Debug.Log($"“{transform.name}” change added {  string.Join(", ", System.Array.ConvertAll(children.added  .ToArray(), static a => $"“{a.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” change removed {string.Join(", ", System.Array.ConvertAll(children.removed.ToArray(), static r => $"“{r.transform.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” change swapped {string.Join(", ", System.Array.ConvertAll(children.swapped.ToArray(), static s => $"“{s.Item1.transform.name}” with “{s.Item2.transform.name}”"))}");
          UnityEngine.Debug.Log($"“{transform.name}” changed; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
        } break;

        case TransformHierarchyObservation.TransformRemoved: {
          children.removed.Capacity = 1;
          enumerator                = transform.GetEnumerator();

          while (true) {
            if (!enumerator.MoveNext()) {
              ++childIndex; // --> childIndex = index + count;
              break;
            }

            if (this.hierarchy[++childIndex].transform != (UnityEngine.Transform) enumerator.Current)
            break;

            UnityEngine.Debug.Log($"Finding removed “{this.hierarchy[childIndex].transform.name}”");

            for (uint subcount = (uint) this.hierarchy[childIndex].childCount; 0u != subcount--; ) {
              UnityEngine.Debug.Log($"  Skip “{this.hierarchy[childIndex + 1].transform.name}” ({this.hierarchy[childIndex + 1].childCount})");
              subcount += (uint) this.hierarchy[++childIndex].childCount;
            }
          }

          child                 = this.hierarchy[childIndex].transform;
          this.hierarchy[index] = (this.hierarchy[index].transform, count - 1);

          for ((count, System.Collections.Generic.Queue<UnityEngine.Transform> hierarchy) = (0, new(new[] {child})); 0 != hierarchy.Count; ++count) {
            foreach (UnityEngine.Transform subtransform in hierarchy.Dequeue())
            hierarchy.Enqueue(subtransform);
          }

          children.removed.Add((child, childIndex));
          this.hierarchy.RemoveRange(childIndex, count);

          UnityEngine.Debug.Log($"“{transform.name}” removed “{child.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
        } break;

        case TransformHierarchyObservation.TransformSwapped: {
          System.Collections.Generic.List<(UnityEngine.Transform, int)> hierarchy = TransformHierarchyObserver.ObserveHierarchy(transform, rooted: false);
          UnityEngine.Transform                                         swappedChild;

          // …
          children.swapped.Capacity = 1;
          enumerator                = transform.GetEnumerator();

          while (true) {
            if (!enumerator.MoveNext())
              goto case TransformHierarchyObservation.TransformChanged;

            child        = (UnityEngine.Transform) enumerator.Current;
            swappedChild = this.hierarchy[++childIndex].transform;

            if (child != swappedChild)
            break;

            UnityEngine.Debug.Log($"Finding swapped “{this.hierarchy[childIndex].transform.name}”");
            for (uint subcount = (uint) this.hierarchy[childIndex].childCount; 0u != subcount--; ) {
              UnityEngine.Debug.Log($"  Skip “{this.hierarchy[childIndex + 1].transform.name}” ({this.hierarchy[childIndex + 1].childCount})");
              subcount += (uint) this.hierarchy[++childIndex].childCount;
            }
          }

          children.swapped.Add(((swappedChild, childIndex), (child, -1)));

          for (childIndex = hierarchy.Count; 0 != childIndex--; )
            this.hierarchy[childIndex + index] = hierarchy[childIndex];

          UnityEngine.Debug.Log($"“{transform.name}” swapped “{swappedChild.name}” with “{child.name}”; Observed {string.Join(", ", this.hierarchy.ConvertAll(static h => $"“{h.transform.name}” ({h.childCount})"))}");
        } break;
      }

      // NOTIFY EVENT LISTENER
      return;
    }
  }


  // private void OnTransformParentChanged() {
  // }
}
