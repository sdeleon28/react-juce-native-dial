# react-juce-native-dial

Dial writen in C++ for react-juce. This implementation avoids any kind of jitter that you may experience while turning a dial because it doesn't send values over the bridge as the user drags.

There's no properly packaged build for this, so you'll have to copy files into the appropriate places.

You'll have to add your own `dial.png` asset.

There is a class called `EditorWithCustomViews`. I don't remember if it's absolutely necessary to do that. I think I needed it to run some code on bundle evaluation. I'm including it just in case.

Before you use `NativeKnob` on the js side, you need to make sure your custom views are registered, like so:

```cpp
auto* editor = new EditorWithCustomViews(*this, bundle);

editor->registerViewType("DiscreteDialMarks", [this] () -> ViewManager::ViewPair {
    auto view = std::make_unique<DiscreteDialMarks>(treeState);
    auto shadowView = std::make_unique<ShadowView>(view.get());
    return {std::move(view), std::move(shadowView)};
});
editor->registerViewType("DialImage", [this] () -> ViewManager::ViewPair {
    auto view = std::make_unique<DialImage>(treeState);
    auto shadowView = std::make_unique<ShadowView>(view.get());
    return {std::move(view), std::move(shadowView)};
});
```
