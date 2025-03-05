var object = {'a': 1, 'b': 2, 'c': 3};
var proxy  = new Proxy(object, {
  // ->> All these operations are “nop”
  // ->> See `Proxy.revocable(…)`
  apply:                    (target, argumentTarget, arguments) => `@apply`,                      // ->> Will override `target` function invocation; affected by `target` type e.g. `function`
  construct:                (target,                 arguments) => ({}),                          // ->> Must return non-null `object`; affected by `target` type e.g. `function`
  defineProperty:           (target, property, descriptor)      => `@define \`"${property}"\``,   //
  deleteProperty:           (target, property)                  => `@delete \`"${property}"\``,   // ->> Converted to `boolean`
  get:                      (target, property, proxy)           => `@get \`"${property}"\``,      //
  getOwnPropertyDescriptor: (target, property)                  => undefined,                     // ->> Must return non-null `object` or `undefined`; affects `ownKeys(…)` handle
  getPrototypeOf:           (target)                            => null,                          // ->> Must return `object`
  has:                      (target, property)                  => `@has \`"${property}"\``,      // ->> Converted to `boolean`
  isExtensible:             (target)                            => `@extensibility`,              // ->> Converted to `boolean`
  ownKeys:                  (target)                            => ['a', 'c', 'e'],               // ->> Needn’t be `Array`, only array-like of strings; affected by `target` type e.g. `function`
  preventExtensions:        (target)                            => void `@prevent extensibility`, // ->> Must return actual extensibility of `target`?
  set:                      (target, property, value)           => `@set \`"${property}"\``,      //
  setPrototypeOf:           (target, prototype)                 => `@set __proto__`,              //
});

/* ... */
console.log("[APPLY]:",     (() => { try { return     proxy() } catch (error) { return error } })());
console.log("[CONSTRUCT]:", (() => { try { return new proxy }   catch (error) { return error } })());
console.log("[DEFINE]:",    Object.defineProperty(proxy, 'd', {}));
console.log("[DELETE]:",    delete proxy['b']);
console.log("[GET]:",       proxy['b']);
console.log("[GET DESC.]:", Object.getOwnPropertyDescriptor(proxy));
console.log("[GET PROTO]:", Object.getPrototypeOf(proxy)); // --> `proxy.__proto__` invokes the `get(…)` handle instead
console.log("[HAS]:",       'b' in proxy);
console.log("[IS EXT.]:",   Object.isExtensible(proxy));

try {
  for (var property in proxy)         // --> Object.keys(…)
  console.log("[OWN KEY]:", property) // --> 'a' 'c'
} catch (error) { console.log("[OWN KEY]:", error) }

console.log("[OWN KEYS]:",     (() => { try { return Object.keys             (proxy) } catch (error) { return error } })());
console.log("[PREVENT EXT.]:", (() => { try { return Object.preventExtensions(proxy) } catch (error) { return error } })());
console.log("[SET]:",          proxy['b'] = 4);
console.log("[SET]:",          proxy['d'] = 4);
console.log("[SET PROTO]:",    Object.setPrototypeOf(proxy, function() {})); // --> `proxy.__proto__ = …` invokes the `set(…)` handle instead

console.dir(proxy);

// ...
object
