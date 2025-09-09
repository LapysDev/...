void function(define) {
    var description = {
        configurable: false,
        enumerable: true,
        get: function() { console.log("[accessor call]"); return "[...]" },
        set: function(value) { console.log("[mutator call]"); return "[...]" }
    };

    define(Function.prototype, "prototype", descriptor);
    define(Object.prototype, "constructor", descriptor);
    define(Object.prototype, "__defineGetter__", descriptor);
    define(Object.prototype, "__defineSetter__", descriptor);
    define(Object.prototype, "__lookupGetter__", descriptor);
    define(Object.prototype, "__lookupSetter__", descriptor);
    define(Object.prototype, "__proto__", descriptor);
    define(Object, "defineProperty", descriptor);
    define(Object, "getOwnPropertyDescriptor", descriptor);
    define(Object, "getOwnPropertyDescriptors", descriptor);
    define(Object, "getPrototypeOf", descriptor);
    define(Object, "setPrototypeOf", descriptor);
    define(window, "Object", descriptor)
}(Object.defineProperty || function(object, identifier, descriptor) { "value" in descriptor ? object[identifier] = descriptor["value"] : object[identifier] = null });
