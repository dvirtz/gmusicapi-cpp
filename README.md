# gmusicapi-cpp
[![Travis Status](https://travis-ci.org/dvirtz/gmusicapi-cpp.png?branch=master)](https://travis-ci.org/dvirtz/gmusicapi-cpp)
[![AppVeyor status](https://ci.appveyor.com/api/projects/status/5lxyvxxk9d9hc0um?svg=true
)](https://ci.appveyor.com/project/dvirtz/gmusicapi-cpp)

A C++ wrapper for Simon Weber's [google music API library](https://github.com/simon-weber/gmusicapi).

```c++
GMusicApi::Module module;
GMusicApi::Mobileclient mc(module);
mc.login(username, password);

auto library = mc.get_all_songs();
GMusicApi::identifiers sweet_track_ids;
for (auto&& song : library)
{
	if (song.artist == "The Cat Empire")
	{
		sweet_track_ids.push_back(song.id);
	}
}

auto playlist_id = mc.create_playlist("Rad muzak");
mc.add_songs_to_playlist(playlist_id, sweet_track_ids);
```

Uses [pybind11](https://github.com/pybind/pybind11) to translate C++ code to python and back.
