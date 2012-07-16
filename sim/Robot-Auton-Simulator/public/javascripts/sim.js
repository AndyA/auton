$(function() {
  function fixUp(svg) {
    st = new SVGTool(svg);
    $('.m_wheel').each(function() {
      st.localise(this, $(this).hasClass('m_left') ? st.CR : st.CL);
    });
    $('#m_vehicle').each(function() {
      st.localise(this);
    });

    $('.m_front').attr({
      transform: 'rotate(30)'
    });
    $('#m_vehicle').attr({
      transform: 'rotate(-30)'
    });
  }

  $('#arena').svg({
    loadURL: 'models/buggy.svg',
    onLoad: function(e) {
      fixUp(e);
    },
  });
});
