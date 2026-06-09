#include <stdio.h>
#include <stdlib.h>

#define W 60
#define H 20

typedef enum { LINE, RECT, TRI, CIRCLE } Type;

typedef struct {
    int id;
    Type type;
    int d[6];
} Shape;

char canvas[H][W];
Shape shapes[100];

int count = 0;
int id_counter = 1;

/* Clear Canvas */
void clear_canvas() {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            canvas[y][x] = '_';
        }
    }
}

/* Plot a Point */
void plot(int x, int y) {
    if (x >= 0 && x < W && y >= 0 && y < H) {
        canvas[y][x] = '*';
    }
}

/* Draw Line using Bresenham Algorithm */
void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;
    int e2;

    while (1) {
        plot(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/* Improved Circle */
void draw_circle(int xc, int yc, int r) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            int dx = x - xc;
            int dy = (y - yc) * 2;   /* Aspect ratio correction */

            int dist = dx * dx + dy * dy;
            int rr = r * r;

            if (dist >= rr - r && dist <= rr + r) {
                plot(x, y);
            }
        }
    }
}

/* Display Canvas */
void display() {

    clear_canvas();

    for (int i = 0; i < count; i++) {

        Shape *s = &shapes[i];

        if (s->type == LINE) {

            draw_line(s->d[0], s->d[1],
                      s->d[2], s->d[3]);

        } else if (s->type == RECT) {

            int x = s->d[0];
            int y = s->d[1];
            int w = s->d[2];
            int h = s->d[3];

            draw_line(x, y, x + w - 1, y);
            draw_line(x, y + h - 1, x + w - 1, y + h - 1);
            draw_line(x, y, x, y + h - 1);
            draw_line(x + w - 1, y, x + w - 1, y + h - 1);

        } else if (s->type == TRI) {

            draw_line(s->d[0], s->d[1],
                      s->d[2], s->d[3]);

            draw_line(s->d[2], s->d[3],
                      s->d[4], s->d[5]);

            draw_line(s->d[4], s->d[5],
                      s->d[0], s->d[1]);

        } else if (s->type == CIRCLE) {

            draw_circle(s->d[0], s->d[1], s->d[2]);
        }
    }

    printf("\n    ");
    for (int x = 0; x < W; x++) {
        if (x % 10 == 0)
            printf("%-10d", x);
    }

    printf("\n    ");

    for (int x = 0; x < W; x++) {
        if (x % 5 == 0)
            printf("|");
        else
            printf(" ");
    }

    printf("\n");

    for (int y = 0; y < H; y++) {
        printf("%2d |", y);

        for (int x = 0; x < W; x++) {
            putchar(canvas[y][x]);
        }

        printf("\n");
    }
}

/* Add Shape */
void add_shape(Type type,
               int d0, int d1,
               int d2, int d3,
               int d4, int d5) {

    if (count >= 100)
        return;

    shapes[count].id = id_counter++;
    shapes[count].type = type;

    shapes[count].d[0] = d0;
    shapes[count].d[1] = d1;
    shapes[count].d[2] = d2;
    shapes[count].d[3] = d3;
    shapes[count].d[4] = d4;
    shapes[count].d[5] = d5;

    count++;
}

/* Delete Shape */
void delete_shape(int id) {

    int idx = -1;

    for (int i = 0; i < count; i++) {
        if (shapes[i].id == id) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {

        for (int i = idx; i < count - 1; i++) {
            shapes[i] = shapes[i + 1];
        }

        count--;
        printf("Shape Deleted Successfully!\n");
    } else {
        printf("Shape ID Not Found!\n");
    }
}

/* Modify Shape */
void modify_shape(int id,
                  int d0, int d1,
                  int d2, int d3,
                  int d4, int d5) {

    for (int i = 0; i < count; i++) {

        if (shapes[i].id == id) {

            shapes[i].d[0] = d0;
            shapes[i].d[1] = d1;
            shapes[i].d[2] = d2;
            shapes[i].d[3] = d3;
            shapes[i].d[4] = d4;
            shapes[i].d[5] = d5;

            printf("Shape Modified Successfully!\n");
            return;
        }
    }

    printf("Shape ID Not Found!\n");
}

/* Main Program */
int main() {

    int choice, id;
    int d0, d1, d2, d3, d4, d5;

    while (1) {

        display();

        printf("\n--- Object Registry ---\n");

        for (int i = 0; i < count; i++) {

            Shape *s = &shapes[i];

            if (s->type == LINE) {
                printf("ID:%d | Line | (%d,%d) -> (%d,%d)\n",
                       s->id, s->d[0], s->d[1], s->d[2], s->d[3]);
            }
            else if (s->type == RECT) {
                printf("ID:%d | Rectangle | x=%d y=%d w=%d h=%d\n",
                       s->id, s->d[0], s->d[1], s->d[2], s->d[3]);
            }
            else if (s->type == TRI) {
                printf("ID:%d | Triangle\n", s->id);
            }
            else if (s->type == CIRCLE) {
                printf("ID:%d | Circle | center=(%d,%d) r=%d\n",
                       s->id, s->d[0], s->d[1], s->d[2]);
            }
        }

        printf("\n1. Add Line");
        printf("\n2. Add Rectangle");
        printf("\n3. Add Triangle");
        printf("\n4. Add Circle");
        printf("\n5. Modify Shape");
        printf("\n6. Delete Shape");
        printf("\n7. Exit");

        printf("\n\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {

            printf("Enter x0 y0 x1 y1: ");
            scanf("%d%d%d%d", &d0, &d1, &d2, &d3);

            add_shape(LINE, d0, d1, d2, d3, 0, 0);

        } else if (choice == 2) {

            printf("Enter x y width height: ");
            scanf("%d%d%d%d", &d0, &d1, &d2, &d3);

            add_shape(RECT, d0, d1, d2, d3, 0, 0);

        } else if (choice == 3) {

            printf("Enter x0 y0 x1 y1 x2 y2: ");
            scanf("%d%d%d%d%d%d",
                  &d0, &d1, &d2, &d3, &d4, &d5);

            add_shape(TRI, d0, d1, d2, d3, d4, d5);

        } else if (choice == 4) {

            printf("Enter center_x center_y radius: ");
            scanf("%d%d%d", &d0, &d1, &d2);

            add_shape(CIRCLE, d0, d1, d2, 0, 0, 0);

        } else if (choice == 5) {

            printf("Enter Shape ID: ");
            scanf("%d", &id);

            printf("Enter six values: ");
            scanf("%d%d%d%d%d%d",
                  &d0, &d1, &d2, &d3, &d4, &d5);

            modify_shape(id, d0, d1, d2, d3, d4, d5);

        } else if (choice == 6) {

            printf("Enter Shape ID: ");
            scanf("%d", &id);

            delete_shape(id);

        } else if (choice == 7) {

            printf("Program Terminated.\n");
            break;
        }
    }

    return 0;
}