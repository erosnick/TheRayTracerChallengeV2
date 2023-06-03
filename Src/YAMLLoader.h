#include <yaml-cpp/yaml.h>

#include "tuple.h"
#include "matrix.h"
#include "material.h"
#include "scene.h"

namespace YAML 
{
	template<>
	struct convert<tuple> 
	{
		static Node encode(const tuple& rhs) 
		{
			Node node;
			
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, tuple& rhs) 
		{
			if (!node.IsSequence() || node.size() != 4) 
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

	template<>
	struct convert<matrix4>
	{
		static Node encode(const matrix4& rhs)
		{
			Node node;
			node.push_back(rhs(0, 0));
			node.push_back(rhs(0, 1));
			node.push_back(rhs(0, 2));
			node.push_back(rhs(0, 3));

			node.push_back(rhs(1, 0));
			node.push_back(rhs(1, 1));
			node.push_back(rhs(1, 2));
			node.push_back(rhs(1, 3));

			node.push_back(rhs(2, 0));
			node.push_back(rhs(2, 1));
			node.push_back(rhs(2, 2));
			node.push_back(rhs(2, 3));

			node.push_back(rhs(3, 0));
			node.push_back(rhs(3, 1));
			node.push_back(rhs(3, 2));
			node.push_back(rhs(3, 3));

			return node;
		}

		static bool decode(const Node& node, matrix4& rhs)
		{
			if (!node.IsSequence() || node.size() != 16)
			{
				return false;
			}

			rhs(0, 0) = node[0].as<float>();
			rhs(0, 1) = node[1].as<float>();
			rhs(0, 2) = node[2].as<float>();
			rhs(0, 3) = node[3].as<float>();

			rhs(1, 0) = node[4].as<float>();
			rhs(1, 1) = node[5].as<float>();
			rhs(1, 2) = node[6].as<float>();
			rhs(1, 3) = node[7].as<float>();

			rhs(2, 0) = node[8].as<float>();
			rhs(2, 1) = node[9].as<float>();
			rhs(2, 2) = node[10].as<float>();
			rhs(2, 3) = node[11].as<float>();

			rhs(3, 0) = node[12].as<float>();
			rhs(3, 1) = node[13].as<float>();
			rhs(3, 2) = node[14].as<float>();
			rhs(3, 3) = node[15].as<float>();

			return true;
		}
	};

	template<>
	struct convert<Material> {
		static Node encode(const Material& rhs) 
		{
			Node node;
			node["color"] = rhs.color;
			node["ambient"] = rhs.ambient;;
			node["diffuse"] = rhs.diffuse;
			node["specular"] = rhs.specular;
			node["shininess"] = rhs.shininess;

			return node;
		}

		static bool decode(const Node& node, Material& rhs) 
		{
			printf("%d\n", static_cast<int32_t>(node.size()));

			rhs.color = node["color"].as<tuple>();
			rhs.ambient = node["ambient"].as<float>();
			rhs.diffuse = node["diffuse"].as<float>();
			rhs.specular = node["specular"].as<float>();
			rhs.shininess = node["shininess"].as<float>();

			return true;
		}
	};

	template<>
	struct convert<Sphere> {
		static Node encode(const Sphere& rhs)
		{
			Node node;
			node["center"] = rhs.center;
			node["radius"] = rhs.radius;;
			node["material"] = rhs.material;

			return node;
		}

		static bool decode(const Node& node, Sphere& rhs)
		{
			printf("%d\n", static_cast<int32_t>(node.size()));

			rhs.center = node["center"].as<tuple>();
			rhs.radius = node["radius"].as<float>();
			rhs.material = node["material"].as<Material>();

			auto translation = node["transform"]["translation"].as<tuple>();
			auto rotation = node["transform"]["rotation"].as<tuple>();
			auto scaleFactor = node["transform"]["scale"].as<tuple>();

			auto patternNode = node["material"]["pattern"];

			//if (!patternNode.IsNull())
			//{
			//	auto patternType = static_cast<PatternType>(patternNode["type"].as<uint8_t>());

			//	switch (patternType)
			//	{
			//	case PatternType::Strip:
			//	{
			//		auto color1 = patternNode["color1"].as<tuple>();
			//		auto color2 = patternNode["color2"].as<tuple>();
			//		auto patternTranslation = patternNode["transform"]["translation"].as<tuple>();
			//		auto patternRotation = patternNode["transform"]["rotation"].as<tuple>();
			//		auto patternScale = patternNode["transform"]["scale"].as<tuple>();
			//		auto pattern = createStripPattern(color1, color2);

			//		auto translation = translate(patternTranslation);
			//		auto rotation = rotate(patternRotation);
			//		auto scaleFactor = scale(patternScale);

			//		pattern->setTransform(translation * rotation * scaleFactor);
			//		rhs.material.pattern = pattern;
			//	}
			//	break;
			//	case PatternType::Gradient:
			//		break;
			//	case PatternType::Ring:
			//		break;
			//	case PatternType::Checker:
			//		break;
			//	case PatternType::RadialGradient:
			//		break;
			//	case PatternType::Blend:
			//		break;
			//	default:
			//		break;
			//	}
			//}

			rhs.transform = translate(translation) * rotateX(rotation.x) * rotateX(rotation.x) * rotateX(rotation.x) * scale(scaleFactor);

			return true;
		}
	};

	template<>
	struct convert<Plane> {
		static Node encode(const Plane& rhs)
		{
			Node node;
			node["extent"] = rhs.extentX;
			node["extent"] = rhs.extentZ;
			node["material"] = rhs.material;

			return node;
		}

		static bool decode(const Node& node, Plane& rhs)
		{
			printf("%d\n", static_cast<int32_t>(node.size()));

			rhs.extentX = node["extent"].as<float>();
			rhs.extentZ = node["extent"].as<float>();
			rhs.material = node["material"].as<Material>();

			auto translation = node["transform"]["translation"].as<tuple>();
			auto rotation = node["transform"]["rotation"].as<tuple>();
			auto scaleFactor = node["transform"]["scale"].as<tuple>();

			auto patternNode = node["material"]["pattern"];

			//if (!patternNode.IsNull())
			//{
			//	auto patternType = static_cast<PatternType>(patternNode["type"].as<uint8_t>());

			//	switch (patternType)
			//	{
			//	case PatternType::Strip:
			//	{
			//		auto color1 = patternNode["color1"].as<tuple>();
			//		auto color2 = patternNode["color2"].as<tuple>();
			//		auto patternTranslation = patternNode["transform"]["translation"].as<tuple>();
			//		auto patternRotation = patternNode["transform"]["rotation"].as<tuple>();
			//		auto patternScale = patternNode["transform"]["scale"].as<tuple>();
			//		auto pattern = createStripPattern(color1, color2);

			//		auto translation = translate(patternTranslation);
			//		auto rotation = rotate(patternRotation);
			//		auto scaleFactor = scale(patternScale);

			//		pattern->setTransform(translation * rotation * scaleFactor);
			//		rhs.material.pattern = pattern;
			//	}
			//	break;
			//	case PatternType::Gradient:
			//		break;
			//	case PatternType::Ring:
			//		break;
			//	case PatternType::Checker:
			//		break;
			//	case PatternType::RadialGradient:
			//		break;
			//	case PatternType::Blend:
			//		break;
			//	default:
			//		break;
			//	}
			//}

			rhs.transform = translate(translation) * rotateX(rotation.x) * rotateX(rotation.x) * rotateX(rotation.x) * scale(scaleFactor);

			return true;
		}
	};
}

Camera createCamera(const YAML::Node& cameraNode)
{
	auto imageWidth = cameraNode["imageWidth"].as<int32_t>();
	auto imageHeight = cameraNode["imageHeight"].as<int32_t>();
	auto fov = cameraNode["fov"].as<float>();

	Camera camera(imageWidth, imageHeight, radians(fov));

	auto cameraRotation = cameraNode["rotation"].as<tuple>();

	auto from = cameraNode["eye"].as<tuple>();
	auto to = cameraNode["center"].as<tuple>();
	auto up = cameraNode["up"].as<tuple>();

	camera.transform = viewTransform(from, to, up);

	return camera;
}

inline static Scene loadScene(const std::string& path)
{
	Scene scene;

	YAML::Node config = YAML::LoadFile(path);

	auto sceneName = config["scene"]["name"].as<std::string>();

	scene.world.setName(sceneName);

	printf("name:%s\n", sceneName.c_str());

	scene.camera = createCamera(config["scene"]["camera"]);

	auto sphere = config["scene"]["objects"]["sphere"].as<Sphere>();

	scene.world.addObject(std::make_shared<Sphere>(sphere));

	auto plane = config["scene"]["objects"]["plane"].as<Plane>();

	scene.world.addObject(std::make_shared<Plane>(plane));

	for (auto iterator = config["skills"].begin(); iterator != config["skills"].end(); iterator++)
	{
		printf("%s\n", iterator->first.as<std::string>().c_str());
	}

	return scene;
}