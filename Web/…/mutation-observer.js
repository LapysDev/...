// Select the node that will be observed for mutations
const targetNode = document.getElementById('some-id');

// Options for the observer (which mutations to observe)
const config = {
    attributeFilter: [], // NOTE (Lapys) -> Array of specified attribute names; otherwise all attributes.
    attributeOldValue: true, // NOTE (Lapys) -> Record previous value of attributes on attribute change within observed nodes.
    attributes: true, // NOTE (Lapys) -> Observe the attributes.
    characterData: true, // NOTE (Lapys) -> Record text change of observed nodes.
    characterDataOldValue: true, // NOTE (Lapys) -> Record previous value of text on text change within observed nodes.
    childList: true, // NOTE (Lapys) -> Observe the children.
    subtree: true // NOTE (Lapys) -> Observe the descendants (along with the other configuration options).
};

// Callback function to execute when mutations are observed
const callback = function(mutationsList, observer) {
    for(let mutation of mutationsList) {
        if (mutation.type === 'childList') {
            console.log('A child node has been added or removed.');
        }
        else if (mutation.type === 'attributes') {
            console.log('The ' + mutation.attributeName + ' attribute was modified.');
        }
    }
};

// Create an observer instance linked to the callback function
const observer = new MutationObserver(callback);

// Start observing the target node for configured mutations
observer.observe(targetNode, config);

// Later, you can stop observing
observer.disconnect();
