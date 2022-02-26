#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <core/AppHarness.h>
#include <core/ReactApplicationRoot.h>

using namespace reactjuce;

//==============================================================================
/** The Editor is a default AudioProcessorEditor with preinstalled functionality
 *  for working with React.
 *
 *  It automatically manages a ReactApplicationRoot, registers some native methods
 *  and properties for interfacing with the editor, and provides some helpful
 *  development tools.
 */
class EditorWithCustomViews
    : public juce::AudioProcessorEditor
    , public juce::AudioProcessorParameter::Listener
    , public juce::Timer
{
public:
    //==============================================================================
    EditorWithCustomViews (juce::AudioProcessor&, const juce::File&);
    ~EditorWithCustomViews() override;

    //==============================================================================
    /** Implement the AudioProcessorParameter::Listener interface. */
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

    //==============================================================================
    /** Start the editor. */
    void start();

    //==============================================================================
    /** Override the timer interface. */
    void timerCallback() override;

    //==============================================================================
    /** Override the component interface. */
    void resized() override;
    void paint (juce::Graphics&) override;
    
    /** Public getter to access appRoot instance */
    ReactApplicationRoot& getReactAppRoot() { return appRoot; }

    void registerViewType(const juce::String & typeId, ViewManager::ViewFactory f);

    /** A quick helper for registering view types. */
    template <typename ViewType, typename ShadowViewType>
    struct GenericViewFactory
    {
        ViewManager::ViewPair operator()() {
            auto view = std::make_unique<ViewType>();
            auto shadowView = std::make_unique<ShadowViewType>(view.get());
            return {std::move(view), std::move(shadowView)};
        }
    };
protected:
    //==============================================================================
    /** ReactApplicationRoot bundle eval callback functions */
    virtual void beforeBundleEvaluated();
    void afterBundleEvaluated();
    //==============================================================================
    std::shared_ptr<EcmascriptEngine>     engine;
    //==============================================================================
private:
    //==============================================================================
    ReactApplicationRoot                  appRoot;
    AppHarness                            harness;

    juce::File                            bundleFile;

    // We keep a map of the parameter IDs and their associated parameter pointers
    // to have a quick lookup in beforeBundleEvaluated where lambdas are called
    // with a param ID
    std::map<juce::String, juce::AudioProcessorParameter*> parameters;

    //==============================================================================
    // The plugin editor holds an array of parameter value readouts which are
    // propagated to the user interface. During parameter value changes on the
    // realtime thread, we capture the values in this array of structs, then at
    // 30Hz propagate the value changes via dispatching events to the jsui.
    struct ParameterReadout {
        std::atomic<float> value = 0.0;
        std::atomic<bool> dirty = false;

        ParameterReadout() = default;

        ParameterReadout(const ParameterReadout& other) {
            value = other.value.load();
            dirty = other.dirty.load();
        }
    };

    std::vector<ParameterReadout> paramReadouts;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorWithCustomViews)
};
