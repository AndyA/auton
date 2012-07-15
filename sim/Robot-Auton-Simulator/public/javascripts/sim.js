$(function() {
  $('#arena').svg({
    loadURL: 'models/buggy.svg',
    onLoad: function(e) {
      $('.m_wheel').each(function() {
        console.log(this);
      }).attr({
        transform: 'rotate(45)'
      });
    },
  });
});
