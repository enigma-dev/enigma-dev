# ENIGMA Visual Shader

> [!NOTE]  
> This project draws inspiration from the [Godot Engine](https://godotengine.org/), particularly in the implementation of visual shader nodes and related functionality. The core ideas are reflected in the following files: ``visual_shader_nodes.h``, ``visual_shader_nodes.cpp``, ``visual_shader.h``, ``visual_shader.cpp``, and ``vs_consts.cpp``. While the concepts are similar, this project diverges from Godot's approach by utilizing the C++ Standard Template Library (STL) instead of Godot's custom API. For context on Godot's decision to avoid STL, see their [FAQ on the topic](https://docs.godotengine.org/en/stable/about/faq.html#doc-faq-why-not-stl). The primary objective of this project is to apply Machine Learning algorithms (Genetic Algorithm) to generate (evolve) a visual shader graph for a given input image. This specific goal motivated the decision to adapt existing implementations rather than developing a completely original solution for visual shader handling.

# TODO

- [ ] Implement a Genetic Algorithm to evolve visual shader graph parameters.
- [ ] Extend the Genetic Algorithm to evolve the visual shader graph nodes and connections (Generate the whole graph).
- [ ] Extend the Genetic Algorithm to evolve the visual shader graphs (both fragment and vertex shaders together).
