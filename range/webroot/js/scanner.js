$(function() {
  var $scanner = $('#scanner');
  var scanner = $scanner[0];

  var WIDTH = 650;
  var HEIGHT = 650;

  var sensor = {
    a: 5461,
    b: -17,
    k: 2
  };

  var curpage;
  var dataset;

  function range_to_distance(r, s) {
    if (r + s.b <= 0) return -1;
    return s.a / (r + s.b) - s.k;
  }

  function plot_orbit(elt, data, idx) {
    var ctx = elt.getContext('2d');
    ctx.save();
    ctx.fillStyle = 'rgba(255, 255, 255, 0.4)';
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    ctx.translate(WIDTH / 2, HEIGHT / 2);

    var set = data[idx];

    var phase = 2 * Math.PI / 3;
    var ps = [0, phase, phase * 2];
    var st = ['rgba(128, 0, 0, 0.6)', 'rgba(0, 128, 0, 0.6)', 'rgba(0, 0, 128, 0.6)'];

    ctx.fillStyle = 'black';
    ctx.fillRect(-5, -10, 10, 20);

    ctx.lineWidth = 4;
    ctx.lineJoin = 'round';

    for (var ds = 0; ds < 3; ds++) {
      ctx.beginPath();
      var series = set.data[ds];
      for (var dp = 0; dp < series.length; dp++) {
        var ts = series[dp][0];
        var vv = series[dp][1];
        var d = range_to_distance(vv, sensor) * 3;
        if (d >= 0) {
          var a = 2 * Math.PI * ts / set.duration + ps[ds];
          var x = Math.sin(a) * d;
          var y = Math.cos(a) * d;
          if (dp == 0) ctx.moveTo(x, y);
          else ctx.lineTo(x, y);
        }
      }
      ctx.strokeStyle = st[ds];
      console.log(ctx.strokeStyle);
      ctx.stroke();
    }
    ctx.restore();
  }

  function first() {
    pause();
    curpage = 0;
    plot_orbit(scanner, dataset, curpage);
  }

  function next() {
    pause();
    if (curpage < dataset.length - 1) curpage++;
    else curpage = 0;
    plot_orbit(scanner, dataset, curpage);
  }

  function prev() {
    pause();
    if (curpage > 0) curpage--;
    else curpage = dataset.length - 1;
    plot_orbit(scanner, dataset, curpage);
  }

  var timer;
  function play() {
    function prime() {
      timer = setTimeout(tf, dataset[curpage].duration / 4);
    }
    function tf() {
      next();
      prime();
    }
    prime();
  }

  function pause() {
    if (timer) clearInterval(timer);
    timer = null;
  }

  $.getJSON('orbits.json', function(data) {
    curpage = 0;
    dataset = data;
    plot_orbit(scanner, dataset, curpage);
  });

  $('#first').click(first);
  $('#prev').click(prev);
  $('#next').click(next);
  $('#play').click(play);
  $('#pause').click(pause);

});
