void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position.y *= -1.0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}
