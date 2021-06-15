// #include <iostream>

// int main() {
//     std::cout << "HW" << std::endl;
//     return 0;
// }

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIX_SOCKET_PATH "/tmp/unix.sock"

// レスポンスの値
enum
{
    OK,
    NG
};

int unix_server();

int main(void)
{

    int ret_code = 0;

    // unixサーバー処理を実行
    ret_code = unix_server();
    if (ret_code != 0)
    {
        printf("error\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int unix_server()
{

    int ret_code = 0;
    void *buf;
    uint32_t buf_len = 0;

    int fd_accept = -1; // 接続受け付け用のFD
    int fd_other = -1;  // sendとかrecv用のFD
    ssize_t size = 0;
    int flags = 0; // MSG_WAITALLとかMSG_NOSIGNALをよく使うけど今回はサンプルなのでフラグは無し

    int response = -1; // レスポンス考えるの面倒だったので0:OK/1:NGで。

    remove(UNIX_SOCKET_PATH); // socket作る前に前回のファイルを消しておく。終了処理でやってもいいけど。

    // ソケットアドレス構造体←今回はここがUNIXドメイン用のやつ
    struct sockaddr_un sun, sun_client;
    memset(&sun, 0, sizeof(sun));
    memset(&sun_client, 0, sizeof(sun_client));

    socklen_t socklen = sizeof(sun_client);

    // UNIXドメインのソケットを作成
    fd_accept = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd_accept == -1)
    {
        printf("1failed to socket(errno:%d, error_str:%s)\n", errno, strerror(errno));
        return -1;
    }

    // ソケットアドレス構造体を設定
    sun.sun_family = AF_LOCAL;              // UNIXドメイン
    strcpy(sun.sun_path, UNIX_SOCKET_PATH); // UNIXドメインソケットのパスを指定

    // 上記設定をソケットに紐づける
    ret_code = bind(fd_accept, (const struct sockaddr *)&sun, sizeof(sun));
    if (ret_code == -1)
    {
        printf("2failed to bind(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd_accept);
        return -1;
    }

    // ソケットに接続待ちを設定する。10はバックログ、同時に何個迄接続要求を受け付けるか。
    ret_code = listen(fd_accept, 10);
    if (ret_code == -1)
    {
        printf("3failed to listen(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd_accept);
        return -1;
    }

    // 無限ループのサーバー処理
    while (1)
    {
        printf("accept wating...\n");
        // 接続受付処理。接続が来るまで無限に待つ。recvのタイムアウト設定時はそれによる。シグナル来ても切れるけど。
        fd_other = accept(fd_accept, (struct sockaddr *)&sun_client, &socklen);
        printf("fd_other\n");
        if (fd_other == -1)
        {
            printf("4failed to accept(errno:%d, error_str:%s)\n", errno, strerror(errno));
            continue;
        }

        // ヘッダーの受信(今回はデータサイズのみだが...)
        char *buf_len2 = (char *)malloc(1000);
        size = recv(fd_other, buf_len2, 1000, flags);
        buf_len = atoi(buf_len2);
        printf("1buf_len = %s\n", buf_len2);
        printf("1size = %d\n", size);
        printf("1buf_len = %d\n", buf_len);
        // if (size < sizeof(buf_len))
        // {
        //     // partial recvパターンも今回はエラーとする
        //     printf("5failed to recv data size(errno:%d, error_str:%s)\n", errno, strerror(errno));
        //     response = NG;
        //     goto send_response;
        // }

        // データ本体の受信用にバッファーを確保
        buf = malloc(buf_len);
        if (buf == NULL)
        {
            printf("6failed to malloc");
            response = NG;
            goto send_response;
        }

        // データ本体の受信
        size = recv(fd_other, buf, buf_len, flags);
        printf("2buf_len = %d\n", buf_len);
        printf("2size = %d\n", size);
        if (size < buf_len)
        {
            // partial recvパターンも今回はエラーとする
            printf("7failed to recv data(errno:%d, error_str:%s)\n", errno, strerror(errno));
            response = NG;
            free(buf);
            goto send_response;
        }

        printf("8received data: [ %s ]\n", buf);
        free(buf);
        response = OK;

    send_response:
        // レスポンスの送信
        size = send(fd_other, &response, sizeof(response), flags);
        if (size < sizeof(response))
        {
            // partial sendパターンも今回はエラーとする
            printf("9failed to send data(errno:%d, error_str:%s)\n", errno, strerror(errno));
        }
        else
        {
            printf("9success to send responce\n");
        }

        // send/recv用のソケットを閉じてFDを-1に戻しておく
        close(fd_other);
        fd_other = -1;
    }

    return 0;
}