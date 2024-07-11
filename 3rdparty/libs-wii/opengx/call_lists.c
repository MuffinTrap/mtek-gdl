/*****************************************************************************
Copyright (c) 2011  David Guillen Fandos (david@davidgf.net)
Copyright (c) 2024  Alberto Mardegan (mardy@users.sourceforge.net)
All rights reserved.

Attention! Contains pieces of code from others such as Mesa and GRRLib

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of copyright holders nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "call_lists.h"
#include "debug.h"
#include "utils.h"

#include <GL/gl.h>
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdlib.h>

#define MAX_COMMANDS_PER_BUFFER 4
#define MAX_CALL_LISTS 1536
/* The glut teapot can take more than 300KB, for reference */
#define MAX_GXLIST_SIZE (1024 * 1024)
#define CALL_LIST_START_ID 1

typedef struct
{
    CommandType type;
    union {
        struct GXDisplayList {
            void *list;
            u32 size;
            struct client_state cs;
        } gxlist;

        GLuint gllist; // glCallList

        GLenum cap; // glEnable, glDisable

        GLenum mode; // glBegin

        struct LightParams {
            uint16_t light;
            uint16_t pname;
            GLfloat params[4];
        } light;

        struct MaterialParams {
            uint16_t face;
            uint16_t pname;
            GLfloat params[4];
        } material;

        struct {
            GLenum sfactor;
            GLenum dfactor;
        } blend_func;

        struct BoundTexture {
            GLenum target;
            GLuint texture;
        } bound_texture;

        struct TexEnv {
            GLenum target;
            GLenum pname;
            GLint param;
        } tex_env;

        struct {
            GLfloat x;
            GLfloat y;
            GLfloat z;
        } xyz;

        struct {
            GLfloat angle;
            GLfloat x;
            GLfloat y;
            GLfloat z;
        } rotate;

        float matrix[16];
    } c;
} Command;

typedef struct CommandBuffer
{
    Command commands[MAX_COMMANDS_PER_BUFFER];
    struct CommandBuffer *next;
} CommandBuffer;

typedef struct
{
    CommandBuffer *head;
} CallList;

static CallList call_lists[MAX_CALL_LISTS];

#define BUFFER_IS_VALID(buffer) (((uint32_t)buffer) > 1)
#define LIST_IS_USED(index) BUFFER_IS_VALID(call_lists[index].head)
#define LIST_IS_RESERVED_OR_USED(index) (call_lists[index].head != NULL)
#define LIST_RESERVE(index) call_lists[index].head = (void*)1
#define LIST_UNRESERVE(index) call_lists[index].head = NULL

static inline int last_command(CommandBuffer **buffer)
{
    CommandBuffer *next;

    while (BUFFER_IS_VALID(*buffer)) {
        next = (*buffer)->next;
        if (!next) {
            for (int i = 0; i < MAX_COMMANDS_PER_BUFFER; i++) {
                Command *curr = &(*buffer)->commands[i];
                if (curr->type == COMMAND_NONE) {
                    return i - 1;
                }
            }
            return MAX_COMMANDS_PER_BUFFER - 1;
        }

        *buffer = next;
    }

    return -1;
}

static Command *new_command(CommandBuffer **head)
{
    int last_index = -1;
    CommandBuffer *last_buffer = NULL;

    if (BUFFER_IS_VALID(*head)) {
        last_buffer = *head;
        last_index = last_command(&last_buffer);
    }

    if (last_index < 0 || last_index == MAX_COMMANDS_PER_BUFFER - 1) {
        CommandBuffer *new_buffer = malloc(sizeof(CommandBuffer));
        if (!new_buffer) {
            warning("Failed to allocate memory for call-list buffer (%d)", errno);
            return NULL;
        }

        for (int i = 0; i < MAX_COMMANDS_PER_BUFFER; i++) {
            new_buffer->commands[i].type = COMMAND_NONE;
        }
        new_buffer->next = NULL;

        if (last_buffer) {
            last_buffer->next = new_buffer;
        } else {
            *head = new_buffer;
        }
        return &new_buffer->commands[0];
    } else {
        return &last_buffer->commands[last_index + 1];
    }
}

static void run_command(Command *cmd)
{
    struct client_state cs;

    switch (cmd->type) {
    case COMMAND_GXLIST:
        cs = glparamstate.cs;
        glparamstate.cs = cmd->c.gxlist.cs;
        _ogx_apply_state();
        glparamstate.cs = cs;
        GX_CallDispList(cmd->c.gxlist.list, cmd->c.gxlist.size);
        break;
    case COMMAND_CALL_LIST:
        glCallList(cmd->c.gllist);
        break;
    case COMMAND_ENABLE:
        glEnable(cmd->c.cap);
        break;
    case COMMAND_DISABLE:
        glDisable(cmd->c.cap);
        break;
    case COMMAND_LIGHT:
        glLightfv(cmd->c.light.light, cmd->c.light.pname, cmd->c.light.params);
        break;
    case COMMAND_MATERIAL:
        glMaterialfv(cmd->c.material.face, cmd->c.material.pname,
                     cmd->c.material.params);
        break;
    case COMMAND_BLEND_FUNC:
        glBlendFunc(cmd->c.blend_func.sfactor, cmd->c.blend_func.dfactor);
        break;
    case COMMAND_BIND_TEXTURE:
        glBindTexture(cmd->c.bound_texture.target,
                      cmd->c.bound_texture.texture);
        break;
    case COMMAND_TEX_ENV:
        glTexEnvi(cmd->c.tex_env.target,
                  cmd->c.tex_env.pname,
                  cmd->c.tex_env.param);
        break;
    case COMMAND_LOAD_IDENTITY:
        glLoadIdentity();
        break;
    case COMMAND_PUSH_MATRIX:
        glPushMatrix();
        break;
    case COMMAND_POP_MATRIX:
        glPopMatrix();
        break;
    case COMMAND_MULT_MATRIX:
        glMultMatrixf(cmd->c.matrix);
        break;
    case COMMAND_TRANSLATE:
        glTranslatef(cmd->c.xyz.x, cmd->c.xyz.y, cmd->c.xyz.z);
        break;
    case COMMAND_ROTATE:
        glRotatef(cmd->c.rotate.angle,
                  cmd->c.rotate.x, cmd->c.rotate.y, cmd->c.rotate.z);
        break;
    case COMMAND_SCALE:
        glScalef(cmd->c.xyz.x, cmd->c.xyz.y, cmd->c.xyz.z);
        break;
    }

}

static void open_gxlist(Command *command)
{
    command->type = COMMAND_GXLIST;
    command->c.gxlist.size = 0;
    command->c.gxlist.list = memalign(32, MAX_GXLIST_SIZE);
    DCInvalidateRange(command->c.gxlist.list, MAX_GXLIST_SIZE);
    /* Save the client state */
    command->c.gxlist.cs = glparamstate.cs;
    GX_BeginDispList(command->c.gxlist.list, MAX_GXLIST_SIZE);
}

static void close_gxlist(Command *command)
{
    u32 size = GX_EndDispList();
    /* Free any excess memory */
    command->c.gxlist.size = size;
    void *new_ptr = realloc(command->c.gxlist.list, size);
    assert(new_ptr == command->c.gxlist.list);

    if (glparamstate.current_call_list.must_execute) {
        GX_CallDispList(command->c.gxlist.list, command->c.gxlist.size);
    }
}

static void close_list(int index)
{
    CallList *list = &call_lists[index];
    if (!LIST_IS_USED(index)) return;

    CommandBuffer *buffer = list->head;
    int i = last_command(&buffer);
    if (i < 0) {
        /* The list is empty */
        return;
    }

    Command *command = &buffer->commands[i];
    if (command->type == COMMAND_GXLIST) {
        close_gxlist(command);
    }

}

static void destroy_buffer(CommandBuffer *buffer)
{
    if (buffer->next) {
        destroy_buffer(buffer->next);
    }

    for (int i =0; i < MAX_COMMANDS_PER_BUFFER; i++) {
        Command *command = &buffer->commands[i];
        if (command->type == COMMAND_NONE) break;

        /* Free the memory for those commands who allocated it */
        if (command->type == COMMAND_GXLIST) {
            void *ptr = command->c.gxlist.list;
            if (ptr) free(ptr);
        }
    }
    free(buffer);
}

static void destroy_list(int index)
{
    CallList *list = &call_lists[index];
    if (!LIST_IS_RESERVED_OR_USED(index)) return;

    if (BUFFER_IS_VALID(list->head)) {
        destroy_buffer(list->head);
    }
    list->head = NULL;
}

/* This function returns true if the caller's code needs to be executed now,
 * false if it can immediately return with no further action.
 *
 * For operations that we store in GX lists we do return true, because we want
 * the caller to perform the operation as usual (the only difference will be
 * that GX will not really execute it, but store it in the GX display list).
 */
bool _ogx_call_list_append(CommandType op, ...)
{
    CallList *list = &call_lists[glparamstate.current_call_list.index];
    CommandBuffer *buffer = list->head;
    Command *command;
    va_list ap;
    int count;

    debug(OGX_LOG_CALL_LISTS, "Adding command %d to list %d",
          op, glparamstate.current_call_list.index);
    int index = last_command(&buffer);
    if (index >= 0) {
        command = &buffer->commands[index];
        if (command->type == COMMAND_GXLIST) {
            if (op == COMMAND_GXLIST) {
                /* If we are already inside a GX display list, nothing to do here */
                return true;
            } else {
                /* End the GX display list operation if one was active */
                close_gxlist(command);
            }
        }
    }

    if (op == COMMAND_GXLIST) {
        command = new_command(&list->head);
        open_gxlist(command);
        return true;
    }

    /* In all other cases, we are adding the command to the list */
    command = new_command(&list->head);
    command->type = op;
    va_start(ap, op);
    switch (op) {
    case COMMAND_CALL_LIST:
        command->c.gllist = va_arg(ap, GLuint);
        break;
    case COMMAND_ENABLE:
    case COMMAND_DISABLE:
        command->c.cap = va_arg(ap, GLenum);
        break;
    case COMMAND_LIGHT:
        command->c.light.light = va_arg(ap, GLenum);
        command->c.light.pname = va_arg(ap, GLenum);
        switch (command->c.light.pname) {
        case GL_CONSTANT_ATTENUATION:
        case GL_LINEAR_ATTENUATION:
        case GL_QUADRATIC_ATTENUATION:
        case GL_SPOT_CUTOFF:
        case GL_SPOT_EXPONENT:
            count = 1;
            break;
        case GL_SPOT_DIRECTION:
            count = 3;
            break;
        case GL_POSITION:
        case GL_DIFFUSE:
        case GL_AMBIENT:
        case GL_SPECULAR:
            count = 4;
            break;
        }
        floatcpy(command->c.light.params, va_arg(ap, GLfloat *), count);
        break;
    case COMMAND_MATERIAL:
        command->c.material.face = va_arg(ap, GLenum);
        command->c.material.pname = va_arg(ap, GLenum);
        switch (command->c.material.pname) {
        case GL_SHININESS:
            count = 1;
            break;
        default:
            count = 4;
            break;
        }
        floatcpy(command->c.material.params, va_arg(ap, GLfloat *), count);
        break;
    case COMMAND_BLEND_FUNC:
        command->c.blend_func.sfactor = va_arg(ap, GLenum);
        command->c.blend_func.dfactor = va_arg(ap, GLenum);
        break;
    case COMMAND_BIND_TEXTURE:
        command->c.bound_texture.target = va_arg(ap, GLenum);
        command->c.bound_texture.texture = va_arg(ap, GLuint);
        break;
    case COMMAND_TEX_ENV:
        command->c.tex_env.target = va_arg(ap, GLenum);
        command->c.tex_env.pname = va_arg(ap, GLenum);
        command->c.tex_env.param = va_arg(ap, GLint);
        break;
    case COMMAND_MULT_MATRIX:
        floatcpy(command->c.matrix, va_arg(ap, GLfloat *), 16);
        break;
    case COMMAND_TRANSLATE:
    case COMMAND_SCALE:
        command->c.xyz.x = va_arg(ap, double);
        command->c.xyz.y = va_arg(ap, double);
        command->c.xyz.z = va_arg(ap, double);
        break;
    case COMMAND_ROTATE:
        command->c.rotate.angle = va_arg(ap, double);
        command->c.rotate.x = va_arg(ap, double);
        command->c.rotate.y = va_arg(ap, double);
        command->c.rotate.z = va_arg(ap, double);
        break;
    }
    va_end(ap);
    return glparamstate.current_call_list.must_execute;
}

GLboolean glIsList(GLuint list)
{
    if (list < CALL_LIST_START_ID) {
        return GL_FALSE;
    }

    list -= CALL_LIST_START_ID;
    if (list >= MAX_CALL_LISTS) {
        return GL_FALSE;
    }

    return LIST_IS_USED(list);
}

void glDeleteLists(GLuint list, GLsizei range)
{
    if (glparamstate.current_call_list.index != -1) {
        set_error(GL_INVALID_OPERATION);
        return;
    }

    if (range < 0) {
        set_error(GL_INVALID_VALUE);
        return;
    }

    for (int i = 0; i < range; i++) {
        int index = list - CALL_LIST_START_ID;
        if (index < 0 || index >= MAX_CALL_LISTS) {
            /* Note that OpenGL does not specify an error in this case */
            break;
        }

        destroy_list(index);
    }
}

GLuint glGenLists(GLsizei range)
{
    int remaining = range;

    for (int i = 0; i < MAX_CALL_LISTS && remaining > 0; i++) {
        if (!LIST_IS_RESERVED_OR_USED(i)) {
            remaining--;
            if (remaining == 0) {
                /* We found a contiguous range available. Reserve them*/
                int first = i - range + 1;
                for (int j = first; j < first + range; j++)
                    LIST_RESERVE(j);
                return first + CALL_LIST_START_ID;
            }
        } else {
            remaining = range;
        }
    }

    if (remaining > 0) {
        warning("Could not allocate %d display lists", remaining);
        set_error(GL_OUT_OF_MEMORY);
    }

    return 0;
}

void glNewList(GLuint list, GLenum mode)
{
    if (list < CALL_LIST_START_ID) {
        set_error(GL_INVALID_VALUE);
        return;
    }

    list -= CALL_LIST_START_ID;
    if (list >= MAX_CALL_LISTS) {
        set_error(GL_INVALID_VALUE);
        return;
    }

    if (glparamstate.current_call_list.index != -1) {
        set_error(GL_INVALID_OPERATION);
        return;
    }

    glparamstate.current_call_list.index = list;
    glparamstate.current_call_list.must_execute = (mode == GL_COMPILE_AND_EXECUTE);
    glparamstate.current_call_list.execution_depth = 0;
    if (LIST_IS_USED(list)) {
        destroy_list(list);
    }
    LIST_RESERVE(list);
}

void glEndList(void)
{
    if (glparamstate.current_call_list.index < 0) {
        set_error(GL_INVALID_OPERATION);
        return;
    }

    close_list(glparamstate.current_call_list.index);
    GLuint list = glparamstate.current_call_list.index + CALL_LIST_START_ID;
    glparamstate.current_call_list.index = -1;
    glparamstate.current_call_list.execution_depth = 0;
}

void glCallList(GLuint id)
{
    if (id < CALL_LIST_START_ID ||
        id - CALL_LIST_START_ID >= MAX_CALL_LISTS) {
        set_error(GL_INVALID_OPERATION);
        return;
    }

    HANDLE_CALL_LIST(CALL_LIST, id);

    debug(OGX_LOG_CALL_LISTS, "Calling list %d", id - CALL_LIST_START_ID);

    bool must_decrement = false;
    if (glparamstate.current_call_list.index >= 0) {
        /* We don't want to expand the call list and put its command inside the
         * list currently building */
        glparamstate.current_call_list.execution_depth++;
        must_decrement = true;
    }

    CallList *list = &call_lists[id - CALL_LIST_START_ID];
    for (CommandBuffer *buffer = list->head;
         BUFFER_IS_VALID(buffer);
         buffer = buffer->next) {
        for (int i =0; i < MAX_COMMANDS_PER_BUFFER; i++) {
            Command *command = &buffer->commands[i];
            if (command->type == COMMAND_NONE) goto done;

            run_command(command);
        }
    }

done:
    if (must_decrement) {
        glparamstate.current_call_list.execution_depth--;
    }
}

void glCallLists(GLsizei n, GLenum type, const GLvoid *lists)
{
    foreach(n, type, lists, glCallList);
}
