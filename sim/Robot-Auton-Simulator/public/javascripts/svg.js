function SVGTool(svg) {
  this.svg = svg;
}

SVGTool.prototype = {
  TL: 0x00,
  TC: 0x01,
  TR: 0x02,
  CL: 0x10,
  CC: 0x11,
  CR: 0x12,
  BL: 0x20,
  BC: 0x21,
  BR: 0x22,
  anchor: function(bb, cent) {
    return {
      x: bb.x + bb.width * (cent & 0x0f) / 2,
      y: bb.y + bb.height * ((cent >> 4) & 0x0f) / 2
    };
  },
  localise: function(elt, cent) {
    var ap = this.anchor(elt.getBBox(), cent);
    var $elt = $(elt);
    var g1 = this.svg.other(elt.parentNode, 'g', {
      transform: 'translate(' + ap.x + ', ' + ap.y + ')'
    });
    var g2 = this.svg.other(g1, 'g', {
      class: $elt.attr('class'),
      id: elt.id
    });
    var g3 = this.svg.other(g2, 'g', {
      transform: 'translate(' + -ap.x + ', ' + -ap.y + ')'
    });

    this.svg.remove(elt);
    $elt.attr({
      class: '',
      id: ''
    });
    this.svg.add(g3, elt);
  }
}
