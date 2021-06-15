const net = require('net')

// UNIXドメインソケットのコネクションを作成する
// net.createConnectionの引数にファイルを指定するとUNIXドメインソケットで繋がる
const client = net.createConnection('/tmp/unix.sock');
client.on('connect', () => {
  console.log('connected.');
});
client.on('data', (data) => {
  console.log(data.toString());
});
client.on('end', () => {
  console.log('disconnected.');
});
client.on('error', (err) => {
  console.error(err.message);
});
// console.log('hello'.length)
client.send

client.write('aiueo'.length.toString());
client.write('aiueo');