#ifndef __ESCENE__
#define __ESCENE__

struct Escene{
    int Init_x;
    int Fim_X;
};

/*
    Movimenta a janela de exibição conforme a posição do player
    -> image_width: tamanho máx da imagem de fundo
    -> screen_width: tamanho da janela 
*/
void UpdateEscene(struct Escene *escene, int player_x, int image_width, int screen_width);

#endif
