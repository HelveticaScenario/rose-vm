#include "rose.h"

void coord_cam_offset(rose_rt_base* r, int16_t* x, int16_t* y) {
    int16_t* camera_offset = (int16_t*) r->camera_offset.begin;
    int16_t x_cam = camera_offset[0];
    int16_t y_cam = camera_offset[1];

    // Offset
    *x -= x_cam;
    *y -= y_cam;
}

rose_api_error rose_rt_base::pset(int16_t x, int16_t y, uint8_t c) {
    coord_cam_offset(this, &x, &y);
    auto screen_width = this->meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH;
    auto screen_height = this->meta.hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT;
    if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        *(screen.begin + (y * screen_width) + x) = c;
        *pen_color_addr = c;
    }
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::pset_default(int16_t x, int16_t y) {
    return pset(x, y, *pen_color_addr);
}

rose_api_error rose_rt_base::pget(int16_t x, int16_t y, uint8_t* res) {
    coord_cam_offset(this, &x, &y);
    auto screen_width = meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH;
    auto screen_height = meta.hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT;
    if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        *res = *(screen.begin + (y * screen_width) + x);
    } else {
        *res = 0;
    }
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::palset(uint8_t idx, rose_color c) {
    auto slot = palette.begin + (idx * 3);
    *slot = c.r;
    *(slot + 1) = c.g;
    *(slot + 2) = c.b;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::palget(uint8_t idx, rose_color* res) {
    auto slot = palette.begin + (idx * 3);
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

rose_api_error rose_rt_base::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    if (y0 == y1) {
        return rectfill(x0, y0, x1, y1, c);
    }

    int16_t dx = (int16_t) abs(x1 - x0), sx = (int16_t) (x0 < x1 ? 1 : -1);
    int16_t dy = (int16_t) abs(y1 - y0), sy = (int16_t) (y0 < y1 ? 1 : -1);
    int16_t err = (int16_t) ((dx > dy ? dx : -dy) / 2), e2;

    for (;;) {
        pset(x0, y0, c);
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
    *pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::line_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return line(x0, y0, x1, y1, *pen_color_addr);
}

rose_api_error rose_rt_base::rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    rect_swap(&x0, &y0, &x1, &y1);
    // Draw
    int i;
    for (i = y0; i <= y1; ++i) {
        pset(x0, (int16_t) i, c);
        pset(x1, (int16_t) i, c);
    }
    for (i = x0; i <= x1; ++i) {
        pset((int16_t) i, y0, c);
        pset((int16_t) i, y1, c);
    }
    *pen_color_addr = c;

    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::rect_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rect(x0, y0, x1, y1, *pen_color_addr);
}

int rect_clip(rose_rt_base* r, int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    auto screen_width = r->meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH;
    auto screen_height = r->meta.hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT;
    if ((*x0 < 0 && *x1 < 0) || *x0 > (screen_width - 1) || (*y0 < 0 && *y1 < 0) ||
        *y0 > (screen_height - 1)) {
        return 1;
    }
    if (*x0 < 0) {
        *x0 = 0;
    }
    if (*x1 > (screen_width - 1)) {
        *x1 = (screen_width - 1);
    }
    if (*y0 < 0) {
        *y0 = 0;
    }
    if (*y1 > (screen_height - 1)) {
        *y1 = (screen_height - 1);
    }
    return 0;
}

rose_api_error rose_rt_base::rectfill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c) {
    rect_swap(&x0, &y0, &x1, &y1);
    coord_cam_offset(this, &x0, &y0);
    coord_cam_offset(this, &x1, &y1);
    int res = rect_clip(this, &x0, &y0, &x1, &y1);
    if (res == 1)
        return ROSE_API_ERR_NONE;
    // Draw
    auto screen_width = meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH;
    for (int i = y0; i <= y1; ++i) {
        memset(screen.begin + (i * screen_width) + x0, c,
                  (size_t) (x1 - x0 + 1));
    }
    *pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::rectfill_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rectfill(x0, y0, x1, y1, *pen_color_addr);
}

rose_api_error rose_rt_base::circ(int16_t x, int16_t y, uint16_t radius, uint8_t c) {
    int16_t offx = radius;
    int16_t offy = 0;
    int16_t decisionOver2 = (int16_t) (1 - offx);   // Decision criterion divided by 2 evaluated at x=r, y=0
    while (offy <= offx) {
        pset(offx + x, offy + y, c); // Octant 1
        pset(offy + x, offx + y, c); // Octant 2
        pset(-offx + x, offy + y, c); // Octant 4
        pset(-offy + x, offx + y, c); // Octant 3
        pset(-offx + x, -offy + y, c); // Octant 5
        pset(-offy + x, -offx + y, c); // Octant 6
        pset(offx + x, -offy + y, c); // Octant 7
        pset(offy + x, -offx + y, c); // Octant 8
        offy++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * offy + 1;   // Change in decision criterion for y -> y+1
        } else {
            offx--;
            decisionOver2 += 2 * (offy - offx) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
    *pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::circ_default(int16_t x0, int16_t y0, uint16_t radius) {
    return circ(x0, y0, radius, *pen_color_addr);
}

rose_api_error rose_rt_base::circfill(int16_t x, int16_t y, uint16_t radius, uint8_t c) {
    int16_t offx = radius;
    int16_t offy = 0;
    int16_t decisionOver2 = (int16_t) (1 - offx);   // Decision criterion divided by 2 evaluated at x=y=0

    while (offy <= offx) {
        line(offx + x, offy + y, -offx + x, offy + y, c); // Octant 1
        line(offy + x, offx + y, -offy + x, offx + y, c); // Octant 2
        line(-offx + x, -offy + y, offx + x, -offy + y, c); // Octant 5
        line(-offy + x, -offx + y, offy + x, -offx + y, c); // Octant 6
        offy++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * offy + 1;   // Change in decision criterion for y -> y+1
        } else {
            offx--;
            decisionOver2 += 2 * (offy - offx) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
    *pen_color_addr = c;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::circfill_default(int16_t x0, int16_t y0, uint16_t radius) {
    return circfill(x0, y0, radius, *pen_color_addr);
}

rose_api_error fill_bottom_flat_triangle(rose_rt_base* r,int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c, bool fill)
{

    int16_t dx1 = (int16_t) abs(x0 - x1), sx1 = (int16_t) (x1 < x0 ? 1 : -1);
    int16_t dy1 = (int16_t) abs(y0 - y1), sy1 = (int16_t) (y1 < y0 ? 1 : -1);
    int16_t err1 = (int16_t) ((dx1 > dy1 ? dx1 : -dy1) / 2), e21;

    int16_t dx2 = (int16_t) abs(x0 - x2), sx2 = (int16_t) (x2 < x0 ? 1 : -1);
    int16_t dy2 = (int16_t) abs(y0 - y2), sy2 = (int16_t) (y2 < y0 ? 1 : -1);
    int16_t err2 = (int16_t) ((dx2 > dy2 ? dx2 : -dy2) / 2), e22;

    for (;;) {
        if (fill && y1 == y2) {
            r->line(x1, y1, x2, y2, c);
        }

        bool changed1 = false;
        for (;;) {
            r->pset(x1, y1, c);
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
            r->pset(x2, y2, c);
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

rose_api_error fill_top_flat_triangle(rose_rt_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t c, bool fill)
{


    int16_t dx1 = (int16_t) abs(x2 - x0), sx1 = (int16_t) (x0 < x2 ? 1 : -1);
    int16_t dy1 = (int16_t) abs(y2 - y0), sy1 = (int16_t) (y0 < y2 ? 1 : -1);
    int16_t err1 = (int16_t) ((dx1 > dy1 ? dx1 : -dy1) / 2), e21;

    int16_t dx2 = (int16_t) abs(x2 - x1), sx2 = (int16_t) (x1 < x2 ? 1 : -1);
    int16_t dy2 = (int16_t) abs(y2 - y1), sy2 = (int16_t) (y1 < y2 ? 1 : -1);
    int16_t err2 = (int16_t) ((dx2 > dy2 ? dx2 : -dy2) / 2), e22;

    for (;;) {
        if (fill && y1 == y0) {
            r->line(x0, y0, x1, y1, c);
        }

        bool changed1 = false;
        for (;;) {
            r->pset(x0, y0, c);
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
            r->pset(x1, y1, c);
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


rose_api_error tricommon(rose_rt_base* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, uint8_t c, bool fill)
{
    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    sortVerticesAscendingByY(&v0_x, &v0_y, &v1_x, &v1_y, &v2_x, &v2_y);

    /* here we know that v1_y <= v2_y <= v3_y */
    /* check for trivial case of bottom-flat triangle */
    if (v1_y == v2_y)
    {
        return fill_bottom_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c, fill);
    }
        /* check for trivial case of top-flat triangle */
    else if (v0_y == v1_y)
    {
        return fill_top_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c, fill);
    }
    else
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
        int16_t v3_x = (int16_t) (v0_x + ((float_t)(v1_y - v0_y) / (float_t)(v2_y - v0_y)) * (v2_x - v0_x));
        int16_t v3_y = v1_y;
        fill_bottom_flat_triangle(r, v0_x, v0_y, v1_x, v1_y, v3_x, v3_y, c, fill);
        fill_top_flat_triangle(r, v1_x, v1_y, v3_x, v3_y, v2_x, v2_y, c, fill);
    }
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::tri(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, uint8_t c)
{
//    line(r, v0_x, v0_y, v1_x, v1_y, c);
//    line(r, v1_x, v1_y, v2_x, v2_y, c);
//    line(r, v2_x, v2_y, v0_x, v0_y, c);
//    return ROSE_API_ERR_NONE;
    return tricommon(this, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c, false);
}

rose_api_error rose_rt_base::tri_default(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y) {
    return tri(v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, *pen_color_addr);
}


rose_api_error rose_rt_base::trifill(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, uint8_t c) {
    return tricommon(this, v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, c, true);
}

rose_api_error rose_rt_base::trifill_default(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y) {
    return trifill(v0_x, v0_y, v1_x, v1_y, v2_x, v2_y, *pen_color_addr);
}

rose_api_error rose_rt_base::cls() {
    memset(screen.begin, 0, screen.end - screen.begin);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::get_spritesheet_meta(uint32_t* addr, uint16_t* sheet_width, uint16_t* sheet_height, uint8_t* sprite_width_mult, uint8_t* sprite_height_mult) {
    uint8_t* sprite_schema = schema.end - ROSE_MEMORY_SPRITE_SCHEMA_SIZE;
    *addr = *((uint32_t*)sprite_schema);
    sprite_schema += 4;
    *sheet_width = *((uint16_t*)sprite_schema);
    sprite_schema += 2;
    *sheet_height = *((uint16_t*)sprite_schema);
    sprite_schema += 1;
    *sprite_width_mult = *sprite_schema;
    sprite_schema += 1;
    *sprite_height_mult = *sprite_schema;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::set_spritesheet_meta(uint32_t addr, uint16_t sheet_width, uint16_t sheet_height, uint8_t sprite_width_mult, uint8_t sprite_height_mult) {
    uint8_t* sprite_schema = schema.end - ROSE_MEMORY_SPRITE_SCHEMA_SIZE;
    *((uint32_t*)sprite_schema) = addr;
    sprite_schema += 4;
    *((uint16_t*)sprite_schema) = sheet_width;
    sprite_schema += 2;
    *((uint16_t*)sprite_schema) = sheet_height;
    sprite_schema += 1;
    *sprite_schema = sprite_width_mult;
    sprite_schema += 1;
    *sprite_schema = sprite_height_mult;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::spr(uint32_t n, int16_t x, int16_t y, uint8_t w, uint8_t h, bool flip_x, bool flip_y) {
    uint32_t addr;
    uint16_t sheet_width;
    uint16_t sheet_height;
    uint8_t sprite_width_mult;
    uint8_t sprite_height_mult;
    get_spritesheet_meta(&addr, &sheet_width, &sheet_height, &sprite_width_mult, &sprite_height_mult);
    uint32_t sheet_x = n % sheet_width;
    uint32_t sheet_y = n / sheet_width;
    if (sheet_y > sheet_height) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    uint32_t sheet_actual_x = sheet_x * sprite_width_mult * 8;
    uint32_t sheet_actual_y = sheet_y * sprite_height_mult * 8;
    uint32_t sheet_horiz_line_length = (uint32_t)sheet_width * (uint32_t)sprite_width_mult * 8;
    uint16_t blit_width = (uint16_t) (sprite_width_mult * w * 8);
    uint16_t blit_height = (uint16_t) (sprite_height_mult * h * 8);

    uint8_t* sheet = mem->data() + addr;
    for (uint16_t y_ = 0; y_ < blit_height; y_++) {
        for (uint16_t x_ = 0; x_ < blit_width; x_++) {
            uint16_t x__ = x_;
            uint16_t y__ = y_;
            if (flip_y) {
                y__ = (uint16_t)abs((int32_t)y_ - (blit_height - 1));
            }
            if (flip_x) {
                x__ = (uint16_t)abs((int32_t)x_ - (blit_width - 1));
            }
            uint8_t c = *(sheet + (sheet_horiz_line_length * (sheet_actual_y + y__)) + sheet_actual_x + x__);
            pset(x + x_, y + y_, c);
        }
    }
    return ROSE_API_ERR_NONE;
}