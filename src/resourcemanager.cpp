#include "resourcemanager.h"
#include "sprite.h"
#include "shaderprogram.h"
#include "level.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <charconv>


ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::ShadersMap ResourceManager::m_shaders;
ResourceManager::LevelsMap ResourceManager::m_levels;
std::string ResourceManager::m_path;

bool ResourceManager::init(const std::string& executable_path)
{
	size_t found = executable_path.find_last_of("/\\");
	if (found == std::string::npos)
		return false;
	m_path = executable_path.substr(0, found);
	return true;
}

void ResourceManager::set_executable_path(const std::string& executable_path)
{
	size_t found = executable_path.find_last_of("/\\");
	if (found == std::string::npos)
		return;
	m_path = executable_path.substr(0, found);
}

void ResourceManager::load_all_resources(const std::string& relative_path)
{
	std::stringstream resources_files(get_file_data(relative_path), std::ios::in);
	while (!resources_files.eof()) {
		std::string str;
		std::getline(resources_files, str);
		if (str.empty())
			continue;
		std::size_t pos = str.find_last_of('\r');
		str = str.substr(0, pos);
		load_resource(str);
	}
}

void ResourceManager::load_resource(const std::string& relative_path)
{
	size_t pos = relative_path.find_last_of('.');
	if (pos == std::string::npos)
		return;
	std::string format = relative_path.substr(pos);
	if (!format.compare(".shr"))
		load_shader(relative_path.substr(0, pos), relative_path);
	else if (!format.compare(".sp"))
		load_sprite(relative_path.substr(0, pos), relative_path);
	else if (!format.compare(".lvl"))
		load_level(relative_path.substr(0, pos), relative_path);
}

void ResourceManager::unload_all_resources()
{
	m_sprites.clear();
	m_shaders.clear();
	m_path.clear();
}

std::shared_ptr<ShaderProgram> ResourceManager::load_shader(const std::string& name, const std::string& relative_path)
{
	if (name.empty())
		return nullptr;

	std::string shader_sources = get_file_data(relative_path);
	if (shader_sources.empty())
		return nullptr;

	std::size_t v_start_pos = shader_sources.find("--vertex") + strlen("--vertex");
	std::size_t f_start_pos = shader_sources.find("--fragment");
	if (v_start_pos == std::string::npos || f_start_pos == std::string::npos)
		return nullptr;
	std::string v_shader_source = shader_sources.substr(v_start_pos, f_start_pos - v_start_pos);
	std::string f_shader_source = shader_sources.substr(f_start_pos + strlen("--fragment"));

	decltype(auto) new_shader = m_shaders.emplace(name, std::make_shared<ShaderProgram>(v_shader_source, f_shader_source)).first->second;
	if (!new_shader->is_compiled()) {
		m_shaders.erase(name);
		std::cerr << "Can't load shader program: " << relative_path << std::endl;
		return nullptr;
	}
	return new_shader;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader(const std::string& name)
{
	auto it = m_shaders.find(name);
	if (it == m_shaders.end()) {
		std::cerr << "Can't find the shader program: " << name << std::endl;
		return nullptr;
	}

	return it->second;
}

std::shared_ptr<Sprite> ResourceManager::load_sprite(const std::string& name, const std::string& relative_description_path)
{
	if (name.empty())
		return nullptr;

	std::stringstream sprite_source(get_file_data(relative_description_path));
	std::string shader_program_name;
	std::getline(sprite_source, shader_program_name);

	decltype(auto) shader = get_shader(shader_program_name);
	if (!shader)
		return nullptr;
	
	return m_sprites.emplace(name, std::make_shared<Sprite>(shader)).first->second;
}

std::shared_ptr<Sprite> ResourceManager::get_sprite(const std::string& name)
{
	auto it = m_sprites.find(name);
	if (it == m_sprites.end()) {
		std::cerr << "Can't find the sprite: " << name << std::endl;
		return nullptr;
	}

	return it->second;
}

void ResourceManager::load_level(const std::string& name, const std::string& relative_path)
{
	m_levels.emplace(name, get_file_data(relative_path));
}

std::string ResourceManager::get_level(const std::string& name)
{
	auto it = m_levels.find(name);
	if (it == m_levels.end())
		return std::string();
	return it->second;
}

std::string ResourceManager::get_file_data(const std::string& relative_file_path)
{
	std::ifstream in;
	in.open(m_path + "/" + relative_file_path, std::ios::in | std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "Failed to open file: " << relative_file_path << std::endl;
		return std::string();
	}
	std::stringstream buffer;
	buffer << in.rdbuf();
	in.close();

	return buffer.str();
}