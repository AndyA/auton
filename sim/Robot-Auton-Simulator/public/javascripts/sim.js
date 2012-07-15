$(function() {
  function fixUp(svg) {
    $('.m_wheel').each(function() {
      var $this = $(this);
      var bb = this.getBBox();
      var cx = bb.x + ($this.hasClass('m_left') ? bb.width : 0);
      var cy = bb.y + bb.height / 2;

      var g1 = svg.other(this.parentNode, 'g', {
        transform: 'translate(' + cx + ', ' + cy + ')'
      });
      var g2 = svg.other(g1, 'g', {
        class: $this.attr('class'),
        id: this.id
      });
      var g3 = svg.other(, 'g', {
        transform: 'translate(' + -cx + ', ' + -cy + ')'
      });

      svg.remove(this);
      $this.attr({
        class: '',
        id: ''
      });
      svg.add(g3, this);

    });
    $('.m_front').attr({
      transform: 'rotate(30)'
    });
  }

  $('#arena').svg({
    loadURL: 'models/buggy.svg',
    onLoad: function(e) {
      fixUp(e);
    },
  });
});
