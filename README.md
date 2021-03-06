# react-juce-native-dial

Dial writen in C++ for react-juce. This implementation avoids any kind of jitter that you may experience while turning a dial because it doesn't send values over the bridge as the user drags.

## Installing

There's no properly packaged build for this, so you'll have to copy files into the appropriate places.

You'll have to add your own `dial.png` asset.

There is a class called `EditorWithCustomViews`. I don't remember if it's absolutely necessary to do that. I think I needed it to run some code on bundle evaluation. I'm including it just in case.

There's also a class called `ParamTooltip`. This could probably have a better name (I used to use this as a tooltip, but then made it replace the dial's label and show the value when the dial is focussed).

## Registering Views

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
editor->registerViewType("ParamTooltip", [this] () -> ViewManager::ViewPair {
    auto view = std::make_unique<ParamTooltip>(treeState);
    auto shadowView = std::make_unique<ShadowView>(view.get());
    return {std::move(view), std::move(shadowView)};
});
```

## Example usage

The following snippet shows a typical `NativeKnob` usage scenario, including focus/blur state management.

This uses a full reducer when it could well be `useState`. Leftover code, I assume. Bear with me :)

```jsx
function reducer(state, action) {
  switch (action.type) {
    case 'set-focus':
      return { ...state, focus: action.payload };
    case 'reset':
      return init(action.payload);
    default:
      throw new Error();
  }
}

const App = () => {
  const [state, dispatch] = useReducer(reducer, { focus: null });
  const setFocussedElement = paramId => {
    dispatch({ type: 'set-focus', payload: paramId });
  };
  // Allows the user to blur element by clicking on the UI's body
  const onMouseDown = () => {
    setFocussedElement(null);
  };
  return (
    <View onMouseDown={onMouseDown}>
      <NativeKnob
        paramId="lows"
        label="LOWS"
        focussedElement={state.focus}
        setFocussedElement={setFocussedElement}
      />
    </View>
  )
```
