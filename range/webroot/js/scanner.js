$(function() {
  var $scanner = $('#scanner');
  var scanner = $scanner[0];

  function step_angle(l) {
    return 2 * Math.PI / l;
  }

  function range_to_distance(r, ds) {
    return (1024 / r) * ds / 20;
  }

  function plot_orbit(elt, data) {
    var ctx = elt.getContext('2d');
    ctx.translate(250, 250);
    var sa = step_angle(data.length);
    var phase = 2 * Math.PI / 3;
    var ps = [0, phase, phase * 2];
    var st = ['rgba(128, 0, 0, 0.6)', 'rgba(0, 128, 0, 0.6)', 'rgba(0, 0, 128, 0.6)'];

    ctx.fillStyle = 'black';
    ctx.fillRect(-5, -10, 10, 20);

    ctx.lineWidth = 4;
    ctx.lineJoin = 'round';

    for (var ds = 0; ds < 3; ds++) {
      ctx.beginPath();
      for (var dp = 0; dp < data.length; dp++) {
        var a = (data.length - 1 - dp) * sa - ps[ds];
        var d = range_to_distance(data[dp][ds], 250);
        var x = Math.sin(a) * d;
        var y = Math.cos(a) * d;
        if (dp == 0) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);
      }
      ctx.strokeStyle = st[ds];
      console.log(ctx.strokeStyle);
      ctx.stroke();
    }
  }

  $.getJSON('orbit.json', function(data) {
    plot_orbit(scanner, data);
  });
});
