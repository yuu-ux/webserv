# 役割分担
- 設定ファイルのパース
- HTTP
    - ソケット通信
    - リクエストパース
    - I/O 多重化（epoll, kqueue)
- CGI
    - fork して指定のプログラムを exec して、レスポンスを HTTP サーバーに戻す
---
# 要件定義
- Reactor
    - epoll
- allowed method
    - GET
    - HEAD
    - POST
    - DELETE
- allowed protcol version
    - 1.1
- allowed header
    - common
        - リクエストボディがある場合必須
            - Content-Length: リクエストボディのサイズ
            - Transfer-Encoding: Content-Length or Transfer-Encoding が必須
            - Content-Type: リクエストボディのコンテントタイプ（送信者がコンテントタイプをわかってる場合必須）
    - request
        - Host (必須, もしなければ400エラーを返す)
    - response
        - Date: レスポンスを作成した日時, 必須
        - Location: 300 番台実装するなら必要
- allowed_config_directives:
    - listen: [server]
    - client_max_body_size: [http, server, location]
    - location: [server]
    - error_page: [http, server, location]
    - limit_except: [location]
    - return: [server, location]
    - root: [http, server, location]
    - autoindex: [http, server, location]
    - index: [http, server, location]
    - upload_store: [http, server, location]
    - server_name: [server]
    - cgi_path: [location] # 要検討
    - cgi_extension: [location] # 要検討
- allowed_status
    - 2xx
        - 200: リクエストが成功しレスポンスがある
        - 201: リクエストが成功し、リソースが作成されたことを示す（今回はPOSTで使うかな）
        - 204: リクエストが成功しレスポンスがない（DELETE など処理は成功したけど、本文がない場合に使用する）
    - 3xx
        - 301: 恒久的なリダイレクト
        - 302: 一時的なリダイレクト
        - 303: POSTでデータを送信後、レスポンスは別の場所にあるとき（PRG パターン POST→GET）
    - 4xx
        - 400: 不正なリクエスト（構文エラー、不正なパラメータ）
        - 403: リソースへの権限なし（パーミッションとか、deny all;とか）
        - 404: リソースが存在しない
        - 405: サポートしてないメソッド（リソースが存在しメソッドを解釈できるが、そのメソッドが許可されてない状態）
        - 408: クライアントが遅い、反応がない
        - 413: コンテンツが大きすぎる
        - 414: URI が長すぎる
    - 5xx
        - 500: サーバーエラー
        - 501: そもそもそのHTTPメソッドをサーバーが実装してない

# http メッセージの構成
リクエストライン: メソッド パス プロトコルバージョン
ヘッダ
空行
ボディ

ステータスライン: プロトコルバージョン ステータスコード ステータスメッセージ
ヘッダ
空行
ボディ

参考：https://developer.mozilla.org/ja/docs/Web/HTTP/Guides/Overview#http_%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8

# メモ
- URI の長さは制限したい（414は対応しない）
- スタータスコードの範囲は100~599まで
- リクエストヘッダーは大文字と小文字を区別しない
- メソッドは大文字のみ許可する
- 各フィールドのサイズを8190バイトに制限
- 1つのリクエストに含まれるヘッダーフィールドは最大100個
- 対応してないヘッダーが来た場合は無視。プログラムは続行。
    - Host が抜けてる場合、400エラー
    - リクエストメッセージの形式が不正な場合も400エラー
    - ヘッダーサイズが大きすぎる場合は431エラー
    - 同じヘッダーが2つある場合、連結できる場合は , 区切りで連結する
        - できない場合は、400エラー
    参考：https://www.ietf.org/rfc/rfc9110.html#name-field-order

## コンテントネゴシエーションとは
同じURIでクライアントに適したリソースを提供できるようにすること
例：User-agent をみて英語圏であれば英語のリソースを提供したり、GIFに対応してないブラウザであればPNGを提供したり
参考：https://en.wikipedia.org/wiki/List_of_HTTP_header_fields

# プログラムの流れ
- 設定ファイル読み込み
- リクエストの受け取り準備（socket, listen, bind 等)
- リクエストを受け取る
    - リクエストをパース
    - GET
        - レスポンスを作る
    - CGI
        - fork して該当のプログラムを実行する
        - レスポンスを作成する
    - POST
        - ファイルアップロード処理が入る？
    - DELETE
        - ファイルの削除処理が入る？
    - クライアントにレスポンスを返す

# ディレクトリ構成
.
├── src/
│   ├── core # 基盤インフラ（エントリーポイント、ユーティリティ関数等）
│   ├── config # 設定ファイル関連（設定ファイルのロード）
│   ├── event # イベント関連（イベントシステムへの登録）
│   └── http # http関連（リクエストの受け取り、レスポンスを作成）
├── test/
│   ├── test.cpp
│   └── ...
├── conf/
│   └── nginx.conf
├── Makefile
├── docs/
│   └── design.md
├── .gitignore
├── .github
├── .clang-tidy
└── .clang-format
