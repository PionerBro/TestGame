#pragma once
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

class Sprite;
class ShaderProgram;
class Level;

class ResourceManager {
public: 
	ResourceManager() = delete;
	~ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;


	static bool init(const std::string& executable_path);
	static void set_executable_path(const std::string& executable_path);
	static void load_all_resources(const std::string& relative_path);
	static void load_resource(const std::string& relative_path);
	static void unload_all_resources();
	static std::shared_ptr<ShaderProgram> load_shader(const std::string& name, const std::string& relative_path);
	static std::shared_ptr<ShaderProgram> get_shader(const std::string& name);
	static std::shared_ptr<Sprite> load_sprite(const std::string& name, const std::string& relative_description_path);
	static std::shared_ptr<Sprite> get_sprite(const std::string& name);
	static void load_level(const std::string& name, const std::string& relative_path);
	static std::string get_level(const std::string& name);
private:
	static std::string get_file_data(const std::string& relative_file_path);
private:
	typedef std::map<std::string, std::shared_ptr<Sprite>> SpritesMap;
	static SpritesMap m_sprites;
	
	typedef std::map<std::string, std::shared_ptr<ShaderProgram>> ShadersMap;
	static ShadersMap m_shaders;

	typedef std::map<std::string, std::string> LevelsMap;
	static LevelsMap m_levels;

	static std::string m_path;
};