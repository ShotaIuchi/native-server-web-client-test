// NG
// const net = require("net");

window.addEventListener("DOMContentLoaded", onLoad);

function onLoad() {
  document.querySelector("#xxxButton1").addEventListener("click", () => {
    console.log("xxxButton1");

    // NG
//     const client = net.createConnection("/tmp/unix.sock");
//     client.on("connect", () => {
//       console.log("connected.");
//     });
//     client.on("data", (data) => {
//       console.log(data.toString());
//     });
//     client.on("end", () => {
//       console.log("disconnected.");
//     });
//     client.on("error", (err) => {
//       console.error(err.message);
//     });

//     client.write("aiueo".length.toString());
//     client.write("aiueo");
  });
  document.querySelector("#xxxButton2").addEventListener("click", () => {
    console.log("xxxButton2");
  });
}
