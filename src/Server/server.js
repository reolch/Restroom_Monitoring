const express = require("express");
const bodyParser = require('body-parser');
const fs = require('fs');

const app  = express();
const port = 3000;

let room1 = "空室"
let room2 = "空室"

// urlencodedとjsonは別々に初期化する
app.use(bodyParser.urlencoded({
    extended: true
}));
app.use(bodyParser.json());

// ルーティングの設定
app.get("/", (req, res) =>{
  console.log(room1);
  console.log(room2);
  res.render('index.ejs', {
    room1: room1,
    room2: room2
  });
  console.log("/ へアクセスがありました");
});

// ルーティングの設定
app.post("/", (req, res) =>{
    // リクエストボディを出力
    console.log(req.body);
    // パラメータ名、nameを出力
    console.log(req.body.name);
    console.log(req.body.state);
    if (req.body.name == "1F-1") {
        room1 = req.body.state;
    }
    if (req.body.name == "1F-2") {
        room2 = req.body.state;
    }
    res.send(req.body.name);
    console.log("/ へアクセスがありました");
  });

// HTTPサーバを起動する
app.listen(port, () => {
  console.log(`listening at http://localhost:${port}`);
});