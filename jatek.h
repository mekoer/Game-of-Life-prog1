#ifndef JATEK_H_INCLUDED
#define JATEK_H_INCLUDED

int** empty(int meret_x);
int** next_state(int meret_x, int** previous_state);
int** input(int meret_x);
void felszabadit(int** tomb, int meret_x);
void ment(int meret_x, int** tomb);
int** illeszt(int meret_x, int** tomb, int uj_meret_x);

#endif // JATEK_H_INCLUDED
