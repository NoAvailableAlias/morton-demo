#version 330 compatibility

in vec4 vertex_color;

void main(void)
{
	gl_FragColor = vertex_color;
}
