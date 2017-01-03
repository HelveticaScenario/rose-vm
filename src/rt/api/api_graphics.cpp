#include "rt/api/api_graphics.h"

void coord_cam_offset(rose_runtime_base* r, int16_t* x, int16_t* y) {
    int16_t* camera_offset = (int16_t*) r->camera_offset->begin;
    int16_t x_cam = camera_offset[0];
    int16_t y_cam = camera_offset[1];

    // Offset
    *x -= x_cam;
    *y -= y_cam;
}

rose_runtime_api_error rose_api_graphics_pset(rose_runtime_base* r, int16_t x, int16_t y, uint8_t c) {
    coord_cam_offset(r, &x, &y);
    if (x >= 0 && x < ROSE_SCREEN_WIDTH && y >= 0 && y < ROSE_SCREEN_HEIGHT) {
        rose_memory_range* screen = r->screen;
        uint8_t* asd = screen->begin;
        *(screen->begin + (y * ROSE_SCREEN_WIDTH) + x) = c;
        *r->pen_color_addr = c;
    }
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_pset_default(rose_runtime_base* r, int16_t x, int16_t y) {
    return rose_api_graphics_pset(r, x, y, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_pget(rose_runtime_base* r, int16_t x, int16_t y, uint8_t* res) {
    coord_cam_offset(r, &x, &y);
    if (x >= 0 && x < ROSE_SCREEN_WIDTH && y >= 0 && y < ROSE_SCREEN_HEIGHT) {
        rose_memory_range* screen = r->screen;
        *res = *(screen->begin + (y * ROSE_SCREEN_WIDTH) + x);
    } else {
        *res = 0;
    }
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_palset(rose_runtime_base* r, uint8_t idx, rose_color c) {
    rose_memory_iterator slot = r->palette->begin + (idx * 3);
    *slot = c.r;
    *(slot + 1) = c.g;
    *(slot + 2) = c.b;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_palget(rose_runtime_base* r, uint8_t idx, rose_color* res) {
    rose_memory_iterator slot = r->palette->begin + (idx * 3);
    res->r = *slot;
    res->g = *(slot + 1);
    res->b = *(slot + 2);
    return ROSE_API_ERR_NONE;
}

void rect_swap(int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    bool swap_x = *x0 > *x1;
    bool swap_y = *y0 > *y1;
    if (swap_x) {
        int16_t tmp = *x0;
        *x0 = *x1;
        *x1 = tmp;
    }
    if (swap_y) {
        int16_t tmp = *y0;
        *y0 = *y1;
        *y1 = tmp;
    }
}

rose_runtime_api_error rose_api_graphics_line(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    if (y0 == y1) {
        return rose_api_graphics_rectfill(r, x0, y0, x1, y1, c);
    }

    int16_t dx = (int16_t) abs(x1 - x0), sx = (int16_t) (x0 < x1 ? 1 : -1);
    int16_t dy = (int16_t) abs(y1 - y0), sy = (int16_t) (y0 < y1 ? 1 : -1);
    int16_t err = (int16_t) ((dx > dy ? dx : -dy) / 2), e2;

    for (;;) {
        rose_api_graphics_pset(r, x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
    *r->pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_line_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rose_api_graphics_line(r, x0, y0, x1, y1, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_rect(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    rect_swap(&x0, &y0, &x1, &y1);
    // Draw
    int i;
    for (i = y0; i <= y1; ++i) {
        rose_api_graphics_pset(r, x0, (int16_t) i, c);
        rose_api_graphics_pset(r, x1, (int16_t) i, c);
    }
    for (i = x0; i <= x1; ++i) {
        rose_api_graphics_pset(r, (int16_t) i, y0, c);
        rose_api_graphics_pset(r, (int16_t) i, y1, c);
    }
    *r->pen_color_addr = c;

    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_rect_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rose_api_graphics_rect(r, x0, y0, x1, y1, *r->pen_color_addr);
}

int rect_clip(int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    if ((*x0 < 0 && *x1 < 0) || *x0 > (ROSE_SCREEN_WIDTH - 1) || (*y0 < 0 && *y1 < 0) ||
        *y0 > (ROSE_SCREEN_HEIGHT - 1)) {
        return 1;
    }
    if (*x0 < 0) {
        *x0 = 0;
    }
    if (*x1 > (ROSE_SCREEN_WIDTH - 1)) {
        *x1 = (ROSE_SCREEN_WIDTH - 1);
    }
    if (*y0 < 0) {
        *y0 = 0;
    }
    if (*y1 > (ROSE_SCREEN_HEIGHT - 1)) {
        *y1 = (ROSE_SCREEN_HEIGHT - 1);
    }
    return 0;
}

rose_runtime_api_error rose_api_graphics_rectfill(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    rect_swap(&x0, &y0, &x1, &y1);
    coord_cam_offset(r, &x0, &y0);
    coord_cam_offset(r, &x1, &y1);
    int res = rect_clip(&x0, &y0, &x1, &y1);
    if (res == 1)
        return ROSE_API_ERR_NONE;
    // Draw
    int i;
    for (i = y0; i <= y1; ++i) {
        memset(r->screen->begin + (i * ROSE_SCREEN_WIDTH) + x0, c,
               (size_t) (x1 - x0 + 1));
    }
    *r->pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_rectfill_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rose_api_graphics_rectfill(r, x0, y0, x1, y1, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_circ(rose_runtime_base* r, int16_t x, int16_t y, uint16_t radius, uint8_t c) {
    int16_t offx = radius;
    int16_t offy = 0;
    int16_t decisionOver2 = (int16_t) (1 - offx);   // Decision criterion divided by 2 evaluated at x=r, y=0
    while (offy <= offx) {
        rose_api_graphics_pset(r, offx + x, offy + y, c); // Octant 1
        rose_api_graphics_pset(r, offy + x, offx + y, c); // Octant 2
        rose_api_graphics_pset(r, -offx + x, offy + y, c); // Octant 4
        rose_api_graphics_pset(r, -offy + x, offx + y, c); // Octant 3
        rose_api_graphics_pset(r, -offx + x, -offy + y, c); // Octant 5
        rose_api_graphics_pset(r, -offy + x, -offx + y, c); // Octant 6
        rose_api_graphics_pset(r, offx + x, -offy + y, c); // Octant 7
        rose_api_graphics_pset(r, offy + x, -offx + y, c); // Octant 8
        offy++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * offy + 1;   // Change in decision criterion for y -> y+1
        } else {
            offx--;
            decisionOver2 += 2 * (offy - offx) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
    *r->pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_circ_default(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius) {
    return rose_api_graphics_circ(r, x0, y0, radius, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_circfill(rose_runtime_base* r, int16_t x, int16_t y, uint16_t radius, uint8_t c) {
    int16_t offx = radius;
    int16_t offy = 0;
    int16_t decisionOver2 = (int16_t) (1 - offx);   // Decision criterion divided by 2 evaluated at x=r, y=0

    while (offy <= offx) {
        rose_api_graphics_line(r, offx + x, offy + y, -offx + x, offy + y, c); // Octant 1
        rose_api_graphics_line(r, offy + x, offx + y, -offy + x, offx + y, c); // Octant 2
        rose_api_graphics_line(r, -offx + x, -offy + y, offx + x, -offy + y, c); // Octant 5
        rose_api_graphics_line(r, -offy + x, -offx + y, offy + x, -offx + y, c); // Octant 6
        offy++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * offy + 1;   // Change in decision criterion for y -> y+1
        } else {
            offx--;
            decisionOver2 += 2 * (offy - offx) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
    *r->pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_circfill_default(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius) {
    return rose_api_graphics_circfill(r, x0, y0, radius, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_tri(rose_runtime_base* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, uint8_t c)
{
    rose_api_graphics_line(r, v0_x, v0_y, v1_x, v1_y, c);
    rose_api_graphics_line(r, v1_x, v1_y, v2_x, v2_y, c);
    rose_api_graphics_line(r, v2_x, v2_y, v0_x, v0_y, c);
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_graphics_tri_default(rose_runtime_base* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y) {
    return rose_api_graphics_tri(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, *r->pen_color_addr);
}


rose_runtime_api_error fill_bottom_flat_triangle(rose_runtime_base* r,int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c)
{

    int16_t dx1 = (int16_t) abs(x0 - x1), sx1 = (int16_t) (x1 < x0 ? 1 : -1);
    int16_t dy1 = (int16_t) abs(y0 - y1), sy1 = (int16_t) (y1 < y0 ? 1 : -1);
    int16_t err1 = (int16_t) ((dx1 > dy1 ? dx1 : -dy1) / 2), e21;

    int16_t dx2 = (int16_t) abs(x0 - x2), sx2 = (int16_t) (x2 < x0 ? 1 : -1);
    int16_t dy2 = (int16_t) abs(y0 - y2), sy2 = (int16_t) (y2 < y0 ? 1 : -1);
    int16_t err2 = (int16_t) ((dx2 > dy2 ? dx2 : -dy2) / 2), e22;

    for (;;) {
        if (y1 == y2) {
            rose_api_graphics_line(r, x1, y1, x2, y2, c);
        }

        bool changed1 = false;
        for (;;) {
            rose_api_graphics_pset(r, x1, y1, c);
            if (x1 == x0 && y1 == y0) break;
            e21 = err1;
            if (e21 > -dx1) {
                err1 -= dy1;
                x1 += sx1;
            }
            if (e21 < dy1) {
                err1 += dx1;
                int16_t new_y = y1 + sy1;
                if (new_y != y1) {
                    y1 = new_y;
                    changed1 = true;
                    break;
                }
            }
        }
        bool changed2 = false;
        for (;;) {
            rose_api_graphics_pset(r, x2, y2, c);
            if (x2 == x0 && y2 == y0) break;
            e22 = err2;
            if (e22 > -dx2) {
                err2 -= dy2;
                x2 += sx2;
            }
            if (e22 < dy2) {
                err2 += dx2;
                int16_t new_y = y2 + sy2;
                if (new_y != y2) {
                    y2 = new_y;
                    changed2 = true;
                    break;
                }
            }
        }
        if (!changed1 && !changed2) {
            break;
        }
    }
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error fill_top_flat_triangle(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c)
{


    int16_t dx1 = (int16_t) abs(x2 - x0), sx1 = (int16_t) (x0 < x2 ? 1 : -1);
    int16_t dy1 = (int16_t) abs(y2 - y0), sy1 = (int16_t) (y0 < y2 ? 1 : -1);
    int16_t err1 = (int16_t) ((dx1 > dy1 ? dx1 : -dy1) / 2), e21;

    int16_t dx2 = (int16_t) abs(x2 - x1), sx2 = (int16_t) (x1 < x2 ? 1 : -1);
    int16_t dy2 = (int16_t) abs(y2 - y1), sy2 = (int16_t) (y1 < y2 ? 1 : -1);
    int16_t err2 = (int16_t) ((dx2 > dy2 ? dx2 : -dy2) / 2), e22;

    for (;;) {
        if (y1 == y0) {
            rose_api_graphics_line(r, x0, y0, x1, y1, c);
        }

        bool changed1 = false;
        for (;;) {
            rose_api_graphics_pset(r, x0, y0, c);
            if (x0 == x2 && y0 == y2) break;
            e21 = err1;
            if (e21 > -dx1) {
                err1 -= dy1;
                x0 += sx1;
            }
            if (e21 < dy1) {
                err1 += dx1;
                int16_t new_y = y0 + sy1;
                if (new_y != y0) {
                    y0 = new_y;
                    changed1 = true;
                    break;
                }
            }
        }
        bool changed2 = false;
        for (;;) {
            rose_api_graphics_pset(r, x1, y1, c);
            if (x1 == x2 && y1 == y2) break;
            e22 = err2;
            if (e22 > -dx2) {
                err2 -= dy2;
                x1 += sx2;
            }
            if (e22 < dy2) {
                err2 += dx2;
                int16_t new_y = y1 + sy2;
                if (new_y != y1) {
                    y1 = new_y;
                    changed2 = true;
                    break;
                }
            }
        }
        if (!changed1 && !changed2) {
            break;
        }

    }
    return ROSE_API_ERR_NONE;
}



void sortVerticesAscendingByY(int16_t* v0_x, int16_t* v0_y, int16_t* v1_x, int16_t* v1_y, int16_t* v2_x, int16_t* v2_y) {
    std::array<std::array<int16_t, 2>, 3> v = {{{*v0_x, *v0_y}, {*v1_x, *v1_y}, {*v2_x, *v2_y}}};
    std::sort(v.begin(), v.end(), [=](auto const& lhs, auto const& rhs){
        return lhs[1] < rhs[1];
    });
    *v0_x = v[0][0];
    *v0_y = v[0][1];
    *v1_x = v[1][0];
    *v1_y = v[1][1];
    *v2_x = v[2][0];
    *v2_y = v[2][1];
}


rose_runtime_api_error rose_api_graphics_trifill(rose_runtime_base* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, uint8_t c)
{
    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    sortVerticesAscendingByY(&v0_x, &v0_y, &v1_x, &v1_y, &v2_x, &v2_y);

    /* here we know that v1_y <= v2_y <= v3_y */
    /* check for trivial case of bottom-flat triangle */
    if (v1_y == v2_y)
    {
        return fill_bottom_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c);
    }
        /* check for trivial case of top-flat triangle */
    else if (v0_y == v1_y)
    {
        return fill_top_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c);
    }
    else
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
        int16_t v3_x = (int16_t) (v0_x + ((float_t)(v1_y - v0_y) / (float_t)(v2_y - v0_y)) * (v2_x - v0_x));
        int16_t v3_y = v1_y;
        fill_bottom_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v3_x, v3_y, c);
        fill_top_flat_triangle(r, v1_x, v1_y, v3_x, v3_y, v2_x, v2_y, c);
    }
//    rose_api_graphics_tri(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c);

}

rose_runtime_api_error rose_api_graphics_trifill_default(rose_runtime_base* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y) {
    return rose_api_graphics_trifill(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, *r->pen_color_addr);
}

rose_runtime_api_error rose_api_graphics_cls(rose_runtime_base* r) {
    memset(r->screen->begin, 0, r->screen->end - r->screen->begin);
    return ROSE_API_ERR_NONE;
}
