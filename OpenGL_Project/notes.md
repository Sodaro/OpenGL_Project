UNIFORM:

    global variable
	can be found using glGetUniformLocation 
	can be updated using glUniform, nf, i, ui, fv

finding a uniform can be done without "using" the shader program first  
updating the uniform can only be done after using the shader program

TEXTURE FUNCTIONS:

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); //subsequent texture commands will affect bound texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, [clamp mode (GL_MIRRORED_REPEAT, GL_REPEAT)])
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, [filtermode (GL_NEAREST, GL_LINEAR etc)])
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture);
    

VECTORS:

    unit vector
        n^ = v¯/ ||v¯||

    dot = v¯ ⋅ k¯ = ||v¯|| ⋅ ||k¯|| ⋅ cosθ
    or component wise multiplication where results are added together
    dot = v.x * k.x + v.y * k.y + v.z * k.z

    cross (only defined in 3D space), produces an orthogonal vector to two non parallel vectors:
    AxB = (
            Ay⋅Bz-Az⋅By
            Az⋅Bx-Ax⋅Bz
            Ax⋅By-Ay⋅Bx
          )

MATRICES:
    
    operations only defined for matrices of the same dimensions
    [1 2] + [5 6] = [1+5 2+6] = [6 8]
    [4 2] - [2 4] = [4 - 2 2 - 4] = [2 -2]
    
scalar * matrix is per element

    2 * [1 2] = [2*1 2*2] = [2 4]

matrix-matrix multiplication

    You can only multiply two matrices if the number of columns on the left-hand side matrix is equal to the number of rows on the right-hand side matrix.
    Matrix multiplication is not commutative that is A⋅B≠B⋅A.

    left matrix rows * right matrix columns

    [1 2] * [5 6] = [1*5 + 2*7  1*6 + 2*8] = [19 22]
    [3 4]   [7 8]   [3*5 + 4*7  3*6 + 4*8]   [43 50]

matrix-vector

    non-uniform scale = scaling factor not same for each axis
    uniform scale = scale equal on all axis

glEnable(GL_DEPTH) for depth buffer checks