// PRNG, limited sequence
var seed = 0; for (var i = 0; i < 10; i++) { seed = (seed * 1664525 + 1013904223) | 0; console.log(seed >>> 0) };

// THE LINEAR CONGRUENTIAL GENERATOR
s=>()=>(2**31-1&(s=Math.imul(48271,s)))/2**31;

// OR
s=>()=>((s=Math.imul(741103597,s))>>>0)/2**32;

// OR
s=>()=>((s=Math.imul(1597334677,s))>>>0)/2**32;
