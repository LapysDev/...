Human = function Human() { /* ... */ };
Person = function Person(name, age) {
    this.age = age || 0;
    this.name = name || null
};
    // Static class methods
    Person.support = function support(personA, personB) { /* ... */ };

    // Extend the class
    Person.prototype = Object.create(Human.prototype);

    // Inherited properties
    Person.prototype.constructor = Person;
    Object.defineProperty(Person.prototype, "id", {
        configurable: true,
        get: function id() { return Math.random() },
        set: function id(id) { /* ... */ }
    })
    Person.prototype.greet = function greet() { return "Hello! I am " + (this.name || "") };

console.log(new Person("Lapys", 18));

// ............................................................................................
Human = class Human {};
Person = class Person extends Human {
    #age = 0; // Private field
    name = null; // Public field

    constructor(name, age) {
        super();
        this.age = age || 0;
        this.name = name || null
    }

    // this instanceof Person -> false
    // this instanceof Human -> true
    static get [Symbol.species]() { return Human }

    // Static class methods
    static support(personA, personB) { /* ... */ }

    // Getters, Setters & Methods
    get id() { return Math.random() }
    set id(id) { /* ... */ }
    greet() { return "Hello! I am " + (this.name || "") }
};
    // Extend the class
    Person.prototype = Object.create(Human.prototype);

console.log(new Person("Jyrsper", 18))
