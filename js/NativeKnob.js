import React, { useState } from "react";
import { View, Text } from "react-juce";
import NativeTooltip from "./NativeTooltip";

function DiscreteDialMarks(props) {
  return React.createElement("DiscreteDialMarks", props, props.children);
}

function DialImage(props) {
  return React.createElement("DialImage", props, props.children);
}

// TODO: Dupe
export const width = 140;
export const knobBoxSize = 130;
const borderWidth = 6;
const separatorPink = 20;
const separatorGreen = 44;
const sliderFillColor = "#66FDCF";
const sliderTrackColor = "#626262";
const floatingBoxWidth = "44%";
const knobImageSize = 72;

const styles = {
  container: {
    marginTop: borderWidth / 2,
    marginBottom: borderWidth / 2,
    paddingTop: separatorPink,
    width,
    flexDirection: 'column',
    alignItems: 'stretch',
  },
  /**
   * I'm using that ugly "top: -30" hack because messing with the knobBox's
   * height screws up the whole radial mark geometry. That's not something
   * I want to deal with right now.
   */
  labelBox: {
    top: -30,
    height: 40,
    width: '100%',
    justifyContent: 'center',
    alignItems: 'center',
  },
  label: {
    fontFamily: 'lato',
    fontSize: 25,
    fontStyle: Text.FontStyleFlags.plain,
    overflow: 'visible',
    justification: Text.JustificationFlags.centred,
    color: '#000000',
  },
  // "top:" adjustment here for same reason as in "labelBox"
  knobBox: {
    top: -1,
    height: knobBoxSize,
    width: knobBoxSize,
    alignSelf: 'center',
    justifyContent: 'center',
    alignItems: 'center',
    overflow: 'visible',
  },
  dial: {
    width: '100%',
    height: '100%',
  },
  image: {
    height: '100%',
    width: '100%',
  },
  slider: {
    width: "100%",
    height: "100%",
  },
  dialImage: {
    top: 10,
    left: 10,
    width: 10,
    height: 10,
    backgroundColor: 'aquamarine',
  },
};

/**
 * OK, I know this looks a bit messy. I'm using two different back-end views for this one element.
 * This makes it a bit more complicated to follow but allows for better behavior of the hovering cursor (if I use just
 * one view, I can't make the cursor change when hovering the dial image).
 *
 * This architecture also allows me to define other types of markings around the dial in the future.
 *
 * You can use this from js land without worrying to much about what's going on under the hood. You should get native
 * performance in React comp form.
 */
const NativeKnob = ({
  paramId,
  label,
  valueLabels,
  focussedElement,
  setFocussedElement,
  isEnum,
  size = 72,
}) => {
  const [knobBoxWidth, setKnobBoxWidth] = useState(0);
  const [knobBoxHeight, setKnobBoxHeight] = useState(0);
  // TODO: Rename to plural
  const focussed = focussedElement !== null && focussedElement.split(",").indexOf(paramId) !== -1;
  const onMouseDown = event => {
    setFocussedElement(paramId);
    event.stopPropagation();
  };
  const onKnobBoxMeasure = ({ width, height }) => {
    setKnobBoxWidth(width);
    setKnobBoxHeight(height);
  };
  const left = knobBoxWidth / 2 - size / 2;
  const top = knobBoxHeight / 2 - size / 2;
  return (
    <View {...styles.container}>
      <View onMeasure={onKnobBoxMeasure}  {...styles.knobBox}>
        <DiscreteDialMarks
          {...styles.dial}
          paramId={paramId}
          size={size}
          labels={valueLabels}
          isEnum={isEnum}
        >
          <DialImage {...styles.dialImage}
            paramId={paramId}
            onMouseDown={onMouseDown}
            size={size}
            left={left}
            top={top}
            focussed={focussed}
            width={size}
            height={size}
            isEnum={isEnum}
          />
        </DiscreteDialMarks>
      </View>
      <View {...styles.labelBox}>
        {focussed ? (
          <NativeTooltip {...styles.label} param={paramId} />
        ) : (
          <Text {...styles.label}>{label}</Text>
        )}
      </View>
    </View>
  );
};

export default NativeKnob;
