#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <math.h>
#include <printf.h>
#include <sys/time.h>


// The total elapsed time since the start of the application.
double start_time;
double elapsed_time;

typedef struct {
    float data[3][3];
} matrix;

float triangle_radius = 0.5;

GLfloat vertices[] = {
        0.0f, 0.5f, 0.0f,     // Apex.
        0.43f, -0.5f, 0.24f,    // Front Right.
        -0.43f, -0.5f, 0.24f,   // Front Left.
        0.0f, -0.5f, -0.5f,   // Back.
};

GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 3,
        1, 2, 3,
};

// With flat shading enabled openGL will only use the colour of the last vertex
// in a shape. That means there's a lot of redundant colours.
GLfloat colors[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
};

double get_current_time() {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (double) tp.tv_sec * 1000.0 + (double) tp.tv_nsec / 1000000.0;
}

void update_elapsed_time() {
    double current_time = get_current_time();
    elapsed_time = current_time - start_time;
}

double normal_sin(double input) {
    return sin(input) + 1.0 / 2.0;
}

void apply_matrix_transform(GLfloat* vertex_pointer, int num_vertices, float matrix[3][3]) {
    for(int vertex_idx = 0; vertex_idx < num_vertices; vertex_idx++) {
        int idx = vertex_idx * 3;

        float result[3] = { 0, 0, 0 };

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result[i] += matrix[i][j] * vertex_pointer[idx + j];
            }
        }
        for (int i = 0; i < 3; i++) {
            vertex_pointer[idx + i] = result[i];
        }
    }
}

void rotate_vertices_around_y_axis(GLfloat* vertex_pointer, int num_vertices, float theta) {
    float rotation_matrix[3][3] = {
        { cos(theta),  0, sin(theta) },
        { 0,           1, 0          },
        { -sin(theta), 0, cos(theta) },
    };
    apply_matrix_transform(vertex_pointer, num_vertices, rotation_matrix);
}

void rotate_vertices_around_x_axis(GLfloat* vertex_pointer, int num_vertices, float theta) {
    float rotation_matrix[3][3] = {
            { 1,           0, 0          },
            { 0, cos(theta), -sin(theta) },
            { 0, sin(theta), cos(theta)  },
    };
    apply_matrix_transform(vertex_pointer, num_vertices, rotation_matrix);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_elapsed_time();

    int num_vertices = 9;
    rotate_vertices_around_y_axis(vertices, num_vertices, 0.03f);
    rotate_vertices_around_x_axis(vertices, num_vertices, 0.0023f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // THe first argument here specifies the size of each vertex/colour, not the number of items in the array.
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    // The total number of items is specified HERE.
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

//    glShadeModel(GL_FLAT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glTranslatef( 0.0003f * elapsed_time, 0.0003f * elapsed_time, 0.0f); // Move the camera 1 unit to the left

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    start_time = get_current_time();

    glutInit(&argc, argv);
    glutCreateWindow("Jacks 3-Dimensional Wonderland");
    glutDisplayFunc(display);
    glutIdleFunc(display);

    // This enables z-buffering so pixels are occluded based on depth.
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

