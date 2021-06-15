// const express = require('express');
// const app = express();
// const http = require('http');
// const server = http.createServer(app);

// app.get('/', (req, res) => {
//   res.send('<h1>Hello world</h1>');
// });

// server.listen(3000, () => {
//   console.log('listening on *:3000');
// });



const net = require('net')
const fs = require('fs')

// サーバーを設定
const server = net.createServer((connection) => {
  console.log('connected.');
  connection.on('close', () => {
    console.log('disconnected.');
  });
  connection.on('data', (data) => {
    console.log(data.toString());
  });
  connection.on('error', (err) => {
    console.error(err.message);
  });
  connection.write('unix domain socket');
  connection.end();
});

// ソケットファイルを削除（存在するとlistenできない）
try {
  fs.unlinkSync('/tmp/unix.sock');
} catch (error) {}

// UNIXドメインソケットでlistenする
server.listen('/tmp/unix.sock');