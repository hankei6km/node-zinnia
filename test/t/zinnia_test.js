/**
 * @file node-zinnia のテスト.
 * @author hankei6km
 * @copyright (c) 2013 hankei6km
 * @license MIT License (http://opensource.org/licenses/mit-license.php)
 */
"use strict";

var helper=require('../test_helper.js');
var QUnit=helper.QUnit;
var assert=helper.assert;
var path = require('path');
var util = require('util');

var zinnia = require('../../index.js');

QUnit.module('node-zinnia');

QUnit.test('recognizer', function() {
  var r = zinnia.Recognizer();

  assert.equal(typeof(r), 'object', 'recognizer instance');

  var or = r.open('/usr/lib/zinnia/model/tomoe/handwriting-ja.model');
  if(!or){
    or = r.open('/usr/local/lib/zinnia/model/tomoe/handwriting-ja.model');
  }
  assert.ok(or, 'model open success');
  or = r.open('./dummy.model');
  assert.ok(!or, 'model open fail');

})

QUnit.test('character', function() {
  var s = zinnia.Character();

  assert.equal(typeof(s), 'object', 'character instance');

  var trychk = function(method){
    var stat = false;
    var args = Array.prototype.slice.call(arguments, 1);
    var err = '';

    try{
      s[method].apply(s, args);
      stat = true;
    }catch(e){
      stat = false;
      err = e.toString();
    }

    return {
      stat: stat,
      err: err
    };
  };

  var chk = trychk('clear');
  assert.ok(chk.stat, 'clear ' + chk.err);
  chk = trychk('set_width', 300);
  assert.ok(chk.stat, 'set_width ' + chk.err);
  chk = trychk('set_height', 300);
  assert.ok(chk.stat, 'set_height ' + chk.err);
  chk = trychk('set_width', '300');
  assert.ok(!chk.stat, 'set_height(invalid arg) ' + chk.err);
  chk = trychk('set_height', '300');
  assert.ok(!chk.stat, 'set_height(invalid arg) ' + chk.err);

  chk = trychk('add', 0, 51, 29);
  assert.ok(chk.stat, 'add(0, 51, 29) ' + chk.err);
  chk = trychk('add', 0, 117, 41);
  assert.ok(chk.stat, 'add(0, 117, 41) ' + chk.err);
  chk = trychk('add', 1, 99, 65);
  assert.ok(chk.stat, 'add(1, 99, 65) ' + chk.err);
  chk = trychk('add', 1, 219, 77);
  assert.ok(chk.stat, 'add(1, 219, 77) ' + chk.err);
  chk = trychk('add', 2, '27', 131);
  assert.ok(!chk.stat, 'add(2, \'27\', 131)(invalid args) ' + chk.err);
})

QUnit.asyncTest('classify', function() {
  var r = zinnia.Recognizer();
  var s = zinnia.Character();
  var tbl = [
    {value:'春', score: -0.46128183603286743},
    {value:'香', score: -0.9831210374832153},
    {value:'佶', score: -1.0071184635162354},
    {value:'背', score: -1.1851245164871216},
    {value:'酋', score: -1.2426910400390625},
    {value:'俺', score: -1.2706456184387207},
    {value:'孝', score: -1.294053077697754},
    {value:'牛', score: -1.3154493570327759},
    {value:'荅', score: -1.3300784826278687},
    {value:'柏', score: -1.3346201181411743},
  ];

  r.open('/usr/lib/zinnia/model/tomoe/handwriting-ja.model');

  s.clear();
  s.set_width(300);
  s.set_height(300);

  s.add(0, 51, 29);
  s.add(0, 117, 41);
  s.add(1, 99, 65);
  s.add(1, 219, 77);
  s.add(2, 27, 131);
  s.add(2, 261, 131);
  s.add(3, 129, 17);
  s.add(3, 57, 203);
  s.add(4, 111, 71);
  s.add(4, 219, 173);
  s.add(5, 81, 161);
  s.add(5, 93, 281);
  s.add(6, 99, 167);
  s.add(6, 207, 167);
  s.add(6, 189, 245);
  s.add(7, 99, 227);
  s.add(7, 189, 227);
  s.add(8, 111, 257);
  s.add(8, 189, 245);

  // Async parallel:
  var complete = 0;

  r.classify(s, 10, function(result){
    var size = result.size();
    assert.equal(size, 10, 'result.size()');
    for(var idx=0; idx<size; idx++){
      assert.equal(result.value(idx), tbl[idx].value,
                   'result.value(' + idx + ')');
      assert.equal(result.score(idx), tbl[idx].score,
                   'result.score(' + idx + ')');
    }
    doneFunc();
  });

  r.classify(s, 1, function(result){
    var size = result.size();
    assert.equal(size, 1, 'result.size() nbest=1');
    assert.equal(result.value(0), tbl[0].value,
                 'result.value(0)');
    assert.equal(result.score(0), tbl[0].score,
                 'result.score(0)');
    doneFunc();
  });

  var doneFunc = function(){
    complete++;
    if(complete >= 2){
      QUnit.start();
    }
  }


  // Sync
  var result = r.classifySync(s, 10);
  var size = result.size();
  assert.equal(size, 10, 'result.size() (Sync)');
  for(var idx=0; idx<size; idx++){
    assert.equal(result.value(idx), tbl[idx].value,
                 'result.value(' + idx + ') (Sync)');
    assert.equal(result.score(idx), tbl[idx].score,
                 'result.score(' + idx + ') (Sync)');
  }

})
