try{
  var zinnia = require("./build/Release/zinnia.node");
}catch(e){
  var zinnia = require("./build/Debug/zinnia.node");
}

exports.Character = zinnia.Character;
exports.Recognizer = zinnia.Recognizer;
