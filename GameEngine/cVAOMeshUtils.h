#ifndef _VAO_MESH_UTILS_HG_
#define _VAO_MESH_UTILS_HG_

class cVAOMeshUtils
{
public:
	~cVAOMeshUtils();
	static cVAOMeshUtils* getInstance(void);
	// Note: the shader program ID is needed to tie 
	// the buffer to the vertex layout of the shader

private:
	cVAOMeshUtils();

	static cVAOMeshUtils* pVAOMeshUtils;
};

#endif // !_VAO_MESH_UTILS_HG_



