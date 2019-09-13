#include "SimpleModel.h"

void SimpleModel::prepare() {

	// モデルデータの読み込み
}

void SimpleModel::render() {

}

void SimpleModel::loadModel(std::string filename)
{
	// Load the model from file using ASSIMP

	const aiScene* scene;
	Assimp::Importer Importer;

	// Flags for loading the mesh
	static const int assimpFlags = aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices;

	scene = Importer.ReadFile(filename.c_str(), assimpFlags);

	// Generate vertex buffer from ASSIMP scene data
	float scale = 1.0f;
	std::vector<Vertex> vertexBuffer;

	// Iterate through all meshes in the file and extract the vertex components
	for (uint32_t m = 0; m < scene->mNumMeshes; m++)
	{
		for (uint32_t v = 0; m < scene->mMeshes[m]->mNumVertices; v++)
		{
			Vertex vertex;

			// Use glm make * functions to convert ASSIMP vectors to glm vectors
			vertex.pos = glm::make_vec3(&scene->mMeshes[m]->mVertices[v].x) * scale;
			vertex.normal = glm::make_vec3(&scene->mMeshes[m]->mNormals[v].x);
			// Texture coordinates and colors may have multiple channels, we only use the first [0] one
			vertex.uv = glm::make_vec2(&scene->mMeshes[m]->mTextureCoords[0][v].x);
			// Mesh may not have vertex colors
			vertex.color = (scene->mMeshes[m]->HasVertexColors(0)) ? glm::make_vec3(&scene->mMeshes[m]->mColors[0][v].r) : glm::vec3(1.0f);

			// Vulkan uses a right-handed NDC (countary to OpenGL), so simply flip y=Axis
			vertex.pos.y *= -1.0f;

			vertexBuffer.push_back(vertex);
		}
	}
	size_t vertexBuffersSize = vertexBuffer.size() * sizeof(Vertex);

	// Generate index buffer from ASSIMP scene data
	std::vector<uint32_t> indexBuffer;
	for (uint32_t m = 0; m < scene->mNumMeshes; m++)
	{
		uint32_t indexBase = static_cast<uint32_t>(indexBuffer.size());
		for (uint32_t f = 0; f < scene->mMeshes[m]->mNumFaces; f++)
		{
			// We assume that all faces are triangulated
			for (uint32_t i = 0; i < 3; i++)
			{
				indexBuffer.push_back(scene->mMeshes[m]->mFaces[f].mIndices[i] + indexBase);
			}
		}
	}
	size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	model.indices.count = static_cast<uint32_t>(indexBuffer.size());

	bool useStaging = true;
	if (useStaging)
	{
		struct 
		{
			VkBuffer buffer;
			VkDeviceMemory memory;
		} vertexStaging, indexStaging;

		// Create staging buffers
		// Vertex data
		//VK_CHECK_RESULT(vulkanDevi)
	}
}