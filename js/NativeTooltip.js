import React from "react";
import { View, Text } from "react-juce";

function ParamTooltip(props) {
  return React.createElement("ParamTooltip", props, props.children);
}

const styles = {
  container: {
    width: "100%",
    height: "100%",
  },
};

const NativeTooltip = ({
  param,
  fontFamily,
  fontSize,
  fontStyle,
  color,
}) => {
  return (
    <ParamTooltip
      {...styles.container}
      param={param}
      fontFamily={fontFamily}
      fontSize={fontSize}
      fontStyle={fontStyle}
      color={color}
    />
  );
};

export default NativeTooltip;
