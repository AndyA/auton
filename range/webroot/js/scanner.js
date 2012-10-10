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

  var opt = {
    zoom: 5,
    normalise: false,
    clip: 70
  };

  function range_to_distance(r, s) {
    if (r + s.b <= 0) return NaN;
    return s.a / (r + s.b) - s.k;
  }

  function avg(ar) {
    var t = 0;
    var c = 0;
    for (var i = 0; i < ar.length; i++) {
      if (!isNaN(ar[i])) {
        t += ar[i];
        c++;
      }
    }
    return t / c;
  }

  function plot_orbit(elt, data, idx, opt) {
    var ctx = elt.getContext('2d');
    ctx.save();
    ctx.fillStyle = 'rgba(255, 255, 255, 0.7)';
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    ctx.translate(WIDTH / 2, HEIGHT / 2);
    ctx.scale(opt.zoom, opt.zoom);

    var set = data[idx];

    var phase = 2 * Math.PI / 3;
    var ps = [0, phase, phase * 2];
    var st = ['rgba(255, 0, 255, 0.1)', 'rgba(255, 255, 0, 0.1)', 'rgba(0, 255, 255, 0.1)'];

    ctx.fillStyle = 'black';
    ctx.fillRect(-4, -5, 8, 10);

    ctx.lineWidth = 0.3;
    ctx.lineJoin = 'round';

    var scale = [1, 1, 1];

    var points = [];
    for (var ds = 0; ds < 3; ds++) {
      var series = set.data[ds];
      points[ds] = [];
      for (var dp = 0; dp < series.length; dp++) {
        var r = range_to_distance(series[dp][1], sensor);
        if (r > opt.clip) r = opt.clip;
        points[ds][dp] = r;
      }
    }

    if (opt.normalise) {
      var ds, savg = [];
      for (ds = 0; ds < 3; ds++) savg[ds] = avg(points[ds]);
      var tavg = avg(savg);
      for (ds = 0; ds < 3; ds++) scale[ds] = tavg / savg[ds];
    }

    for (var ds = 0; ds < 3; ds++) {
      ctx.beginPath();
      var series = set.data[ds];
      for (var dp = 0; dp < series.length; dp++) {
        var ts = series[dp][0];
        var d = points[ds][dp] * scale[ds];
        if (d >= 0) {
          var a = 2 * Math.PI * ts / set.duration + ps[ds];
          var x = Math.sin(a) * d;
          var y = Math.cos(a) * d;
          if (dp == 0) ctx.moveTo(x, y);
          else ctx.lineTo(x, y);
        }
      }
      ctx.closePath();
      ctx.strokeStyle = 'black';
      ctx.fillStyle = st[ds];
      ctx.fill();
      ctx.stroke();
    }
    ctx.restore();
  }

  function first() {
    pause();
    curpage = 0;
    plot_orbit(scanner, dataset, curpage, opt);
  }

  function next() {
    pause();
    if (curpage < dataset.length - 1) curpage++;
    else curpage = 0;
    plot_orbit(scanner, dataset, curpage, opt);
  }

  function prev() {
    pause();
    if (curpage > 0) curpage--;
    else curpage = dataset.length - 1;
    plot_orbit(scanner, dataset, curpage, opt);
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

  function zoom_in() {
    opt.zoom *= 1.2;
    plot_orbit(scanner, dataset, curpage, opt);
  }

  function zoom_out() {
    opt.zoom /= 1.2;
    plot_orbit(scanner, dataset, curpage, opt);
  }

  $.getJSON('orbits.json', function(data) {
    curpage = 0;
    dataset = data;
    plot_orbit(scanner, dataset, curpage, opt);
  });

  $('#first').click(first);
  $('#prev').click(prev);
  $('#next').click(next);
  $('#play').click(play);
  $('#pause').click(pause);
  $('#in').click(zoom_in);
  $('#out').click(zoom_out);

});
