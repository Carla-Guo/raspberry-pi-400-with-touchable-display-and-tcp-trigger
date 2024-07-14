
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/sdl/sdl.h"
#include "ui/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#ifdef __arm__ // 编译时检测是否为ARM架构，即树莓派
#include <pigpio.h>
#endif


/*********************
 *      DEFINES
 *********************/

#ifdef __arm__
#define UART_DEVICE "/dev/serial0"
#define BAUD_RATE 9600
volatile int uart_fd;
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/
int clientSocket;
struct sockaddr_in serverAddr;
char buffer[BUFFER_SIZE];


    //鸡汤语录
const char *random_values[100] = {
    "Reading is to the mind what exercise is to the body. - Joseph Addison",
    "A room without books is like a body without a soul. - Marcus Tullius Cicero",
    "The more that you read, the more things you will know. The more that you learn, the more places you'll go. - Dr. Seuss",
    "Books are a uniquely portable magic. - Stephen King",
    "Reading is an adventure in living. - Clifton Fadiman",
    "The journey of a lifetime starts with the turning of a page. - Rachel Anders",
    "There is no friend as loyal as a book. - Ernest Hemingway",
    "Books are the mirrors of the soul. - Virginia Woolf",
    "Reading is dreaming with open eyes. - Unknown",
    "Books are a uniquely portable magic. - Stephen King",
    "So many books, so little time. - Frank Zappa",
    "A book is a gift you can open again and again. - Garrison Keillor",
    "A book is a dream that you hold in your hand. - Neil Gaiman",
    "The only thing you absolutely have to know is the location of the library. - Albert Einstein",
    "A good book is an event in my life. - Stendhal",
    "Reading is a discount ticket to everywhere. - Mary Schmich",
    "Books can be dangerous. The best ones should be labeled 'This could change your life.' - Helen Exley",
    "Today a reader, tomorrow a leader. - Margaret Fuller",
    "A reader lives a thousand lives before he dies. The man who never reads lives only one. - George R.R. Martin",
    "Books are the quietest and most constant of friends. - Charles William Eliot",
    "Reading is a way for me to expand my mind, open my eyes, and fill up my heart. - Oprah Winfrey",
    "Books are the plane, and the train, and the road. They are the destination, and the journey. They are home. - Anna Quindlen",
    "Books wash away from the soul the dust of everyday life. - Unknown",
    "We read to know we're not alone. - William Nicholson",
    "Reading brings us unknown friends. - Honoré de Balzac",
    "A book is like a garden carried in the pocket. - Chinese Proverb",
    "Books are the treasured wealth of the world and the fit inheritance of generations and nations. - Henry David Thoreau",
    "Reading is a basic tool in the living of a good life. - Mortimer J. Adler",
    "Books serve to show a man that those original thoughts of his aren't very new after all. - Abraham Lincoln",
    "Reading is a window to the world. - Lynn Butler",
    "Books are the training weights of the mind. - Epictetus",
    "Reading is a conversation. All books talk. But a good book listens as well. - Mark Haddon",
    "Books are my friends, my companions. They make me laugh and cry and find meaning in life. - Christopher Paolini",
    "The book to read is not the one that thinks for you but the one which makes you think. - Harper Lee",
    "Reading is a sanctuary where the soul finds peace. - Unknown",
    "Books are the gateway to wisdom and knowledge. - Unknown",
    "A great book should leave you with many experiences, and slightly exhausted at the end. You live several lives while reading. - William Styron",
    "Reading gives us someplace to go when we have to stay where we are. - Mason Cooley",
    "Books open your mind, broaden your mind, and strengthen you as nothing else can. - William Feather",
    "Reading is the key to knowledge. - Unknown",
    "Books are a treasure trove of knowledge and wisdom. - Unknown",
    "There is more treasure in books than in all the pirate's loot on Treasure Island. - Walt Disney",
    "A book is a device to ignite the imagination. - Alan Bennett",
    "Reading makes immigrants of us all. It takes us away from home, but more important, it finds homes for us everywhere. - Jean Rhys",
    "Books are the compasses and telescopes and sextants and charts which other men have prepared to help us navigate the dangerous seas of human life. - Jesse Lee Bennett",
    "Reading gives us a place to go when we have to stay where we are. - Mason Cooley",
    "Books are the bees which carry the quickening pollen from one to another mind. - James Russell Lowell",
    "Reading is to the mind what food is to the body. - Unknown",
    "Books are the greatest invention of the human race. - Barbara Tuchman",
    "Reading allows us to see the world through the eyes of others. - Unknown",
    "Books are a source of wisdom and enlightenment. - Unknown",
    "Reading is the gateway skill that makes all other learning possible. - Barack Obama",
    "Books are the mirrors of the soul. - Virginia Woolf",
    "Reading brings us unknown friends. - Honoré de Balzac",
    "Books are the ultimate Dumpees: put them down and they'll wait for you forever; pay attention to them and they always love you back. - John Green",
    "Reading is a discount ticket to everywhere. - Mary Schmich",
    "Books can be dangerous. The best ones should be labeled 'This could change your life.' - Helen Exley",
    "Today a reader, tomorrow a leader. - Margaret Fuller",
    "A reader lives a thousand lives before he dies. The man who never reads lives only one. - George R.R. Martin",
    "Books are the quietest and most constant of friends. - Charles William Eliot",
    "Reading is a way for me to expand my mind, open my eyes, and fill up my heart. - Oprah Winfrey",
    "Books are the plane, and the train, and the road. They are the destination, and the journey. They are home. - Anna Quindlen",
    "Books wash away from the soul the dust of everyday life. - Unknown",
    "We read to know we're not alone. - William Nicholson",
    "Reading brings us unknown friends. - Honoré de Balzac",
    "A book is like a garden carried in the pocket. - Chinese Proverb",
    "Books are the treasured wealth of the world and the fit inheritance of generations and nations. - Henry David Thoreau",
    "Reading is a basic tool in the living of a good life. - Mortimer J. Adler",
    "Books serve to show a man that those original thoughts of his aren't very new after all. - Abraham Lincoln",
    "Reading is a window to the world. - Lynn Butler",
    "Books are the training weights of the mind. - Epictetus",
    "Reading is a conversation. All books talk. But a good book listens as well. - Mark Haddon",
    "Books are my friends, my companions. They make me laugh and cry and find meaning in life. - Christopher Paolini",
    "The book to read is not the one that thinks for you but the one which makes you think. - Harper Lee",
    "Reading is a sanctuary where the soul finds peace. - Unknown",
    "Books are the gateway to wisdom and knowledge. - Unknown",
    "A great book should leave you with many experiences, and slightly exhausted at the end. You live several lives while reading. - William Styron",
    "Reading gives us someplace to go when we have to stay where we are. - Mason Cooley",
    "Books open your mind, broaden your mind, and strengthen you as nothing else can. - William Feather",
    "Reading is the key to knowledge. - Unknown",
    "Books are a treasure trove of knowledge and wisdom. - Unknown",
    "There is more treasure in books than in all the pirate's loot on Treasure Island. - Walt Disney",
    "A book is a device to ignite the imagination. - Alan Bennett",
    "Reading makes immigrants of us all. It takes us away from home, but more important, it finds homes for us everywhere. - Jean Rhys",
    "Books are the compasses and telescopes and sextants and charts which other men have prepared to help us navigate the dangerous seas of human life. - Jesse Lee Bennett",
    "Reading gives us a place to go when we have to stay where we are. - Mason Cooley",
    "Books are the bees which carry the quickening pollen from one to another mind. - James Russell Lowell",
    "Reading is to the mind what food is to the body. - Unknown",
    "Books are the greatest invention of the human race. - Barbara Tuchman",
    "Reading allows us to see the world through the eyes of others. - Unknown",
    "Books are a source of wisdom and enlightenment. - Unknown",
    "Reading is the gateway skill that makes all other learning possible. - Barack Obama",
    "Books are the mirrors of the soul. - Virginia Woolf",
    "Reading brings us unknown friends. - Honoré de Balzac",
    "Books are the ultimate Dumpees: put them down and they’ll wait for you forever; pay attention to them and they always love you back. - John Green",
    "Reading is a discount ticket to everywhere. - Mary Schmich",
    "Books can be dangerous. The best ones should be labeled 'This could change your life.' - Helen Exley",
    "Today a reader, tomorrow a leader. - Margaret Fuller",
    "A reader lives a thousand lives before he dies. The man who never reads lives only one. - George R.R. Martin",
    "Books are the quietest and most constant of friends. - Charles William Eliot"
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
    //随机选中一条鸡汤语录
int get_random_index() {
    return rand() % 100;
}

    //播放动画
void play_animations() {

    // Get two random values from the array
    int random_index1 = get_random_index();
    int random_index2 = get_random_index();
    const char *random_value1 = random_values[random_index1];
    //const char *random_value2 = random_values[random_index2];

    // Update the text area with the first random value，更改显示语句
    lv_textarea_set_text(ui_Textarera, random_value1);
    // Update thw image with emoj3，更改表情
    lv_img_set_src(ui_Image1, &ui_img_emoj3_png);
    // Run animation1，播放鸡汤动画
    word_Animation(ui_Screen1,0);

    // Check if the values match，如果中奖
    if (random_index1 == random_index2) {
      
      // Update the image with the emoj2，更改表情，更改显示语句为celebration
      lv_img_set_src(ui_Image1, &ui_img_emoj2_png);
      lv_textarea_set_text(ui_Textarera, "\nCELEBRATIONS!");
      // Run animation2，播放中奖动画
      emoji_Animation(ui_Image1,0);
    }


    usleep(5000000);
    // Restore the text area and the image，回归初始页面
    lv_textarea_set_text(ui_Textarera, "\nPRESS IT!");
    lv_img_set_src(ui_Image1, &ui_img_emoj1_png);
}

#ifdef __arm__
void uart_interrupt() {
    char received[13];
    int i = 0;

    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return;
    }

    uart_fd = serOpen(UART_DEVICE, BAUD_RATE, 0);
    if (uart_fd < 0) {
        fprintf(stderr, "Unable to open UART device: %s\n", strerror(errno));
        gpioTerminate();
        return;
    }

    memset(received, 0, sizeof(received));

    while (1) {
        if (serDataAvailable(uart_fd)) {
            char ch = serReadByte(uart_fd);
            if (ch == ' ') continue;
            received[i++] = ch;

            if (i == 12) {
                if (strcmp(received, "000000010011") == 0) {
                    play_animations();
                }
                i = 0;
                memset(received, 0, sizeof(received));
            }
        }
    }

    serClose(uart_fd);
    gpioTerminate();
}
#endif



int tcpclient_init(){
     int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址信息
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <=

 0) {
        perror("Failed to set server IP");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);
        // 发送数据
    strcpy(buffer, "Hello, Server!");
    if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
        perror("Failed to send data");
        exit(EXIT_FAILURE);
    }

    // 接收响应
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == -1) {
        perror("Failed to receive data");
        exit(EXIT_FAILURE);
    }
  return 0;
}
int tcpclient(const char * text){

    // 发送数据
    strcpy(buffer, text);
    if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
        perror("Failed to send data");
        exit(EXIT_FAILURE);
    }

    // 接收响应
    memset(buffer, 0, sizeof(buffer));
    if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == -1) {
        perror("Failed to receive data");
        exit(EXIT_FAILURE);
    }

    printf("Server response: %s\n", buffer);

    return 0;
}

int close_tcpclient(){
    // 关闭套接字
    close(clientSocket);
    return 0;
}

int main(int argc, char **argv)
{
  
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  tcpxlient_init();

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

  ui_init();

#ifdef __arm__
uart_interrupt();
#endif




//  close_tcpclient();

  


  while(1) {
      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(5 * 1000);
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for LVGL
 */
static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_color_t buf[SDL_HOR_RES * SDL_VER_RES];
  static lv_disp_draw_buf_t disp_draw_buf;
  lv_disp_draw_buf_init(&disp_draw_buf, buf, NULL, SDL_HOR_RES * SDL_VER_RES);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_draw_buf;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;
  lv_disp_drv_register(&disp_drv);

  /* Add a mouse as input device */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); /*Basic initialization*/

  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = evdev_read;
  lv_indev_drv_register(&indev_drv);
}
