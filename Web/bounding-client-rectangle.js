function getBoundingClientRect(element) {
  if ("offsetParent" in element) {
    var bounds = {bottom: +0, left: +0, right: +0, top: +0};
    var element = element;

    bounds.bottom = element.offsetHeight;
    bounds.right  = element.offsetWidth;
    do {
      bounds.left += element.offsetLeft;
      bounds.top  += element.offsetTop;

      element = element.offsetParent
    } while (null !== element);

    bounds.bottom += bounds.top;
    bounds.right  += bounds.left;

    return bounds
  }

  return null
}
