<template>
  <div id="colorPicker">
    <h3>Color Picker Page</h3>
    <div id='showColor' :style="{'background-color': bgc}"></div>
    <div id='sliderContainer'>
      <vue-slider v-model="colors"></vue-slider>
    </div>
    <button v-on:click='sendColor'>Update Color</button>
    <!-- It would be a good idea to create a read characteristic of the current led color
    so show what current vs new is on the page.
    Note: I can bind the color of a div to the data of the last color picked -->
  </div>
</template>

<script>
import {eventBus} from './main.js';
import {Slider} from 'vue-color';

let defaultProps = {
  hex: '#194d33',
  hsl: {
    h: 150,
    s: 0.5,
    l: 0.2,
    a: 1
  },
  hsv: {
    h: 150,
    s: 0.66,
    v: 0.30,
    a: 1
  },
  rgba: {
    r: 25,
    g: 77,
    b: 51,
    a: 1
  },
  a: 1
}

export default {
  data: function() {
    return {
      colorService: '0x1234',
      colorWriteCharacteristic: '0x002',
      colorWriteCharRef: 'nothing',
      colors: defaultProps
    }
  },
  mounted() {
    //on eventbus write char ref work
    eventBus.$on('writeCharRef', (characteristic) => {
      console.log('got write characteristic (color picker)');
      this.colorWriteCharRef = characteristic;
    });
  },
  methods: {
    sendColor: function() {
      console.log('sending color to ble feather');
      let colorPacket = new Uint8Array(6);
      colorPacket[0] = '40';
      colorPacket[1] = '1'; // 1 = Color Command
      colorPacket[2] = this.colors.rgba.r.toString(); // Red Color Value
      colorPacket[3] = this.colors.rgba.g.toString(); // Green Color Value
      colorPacket[4] = this.colors.rgba.b.toString(); // Blue Color Value
      console.log("About to send: ");
      console.log("RED: " + colorPacket[2]);
      console.log("GREEN: " + colorPacket[3]);
      console.log("BLUE: " + colorPacket[4]);
      this.colorWriteCharRef.writeValue(colorPacket);
      return;
    }
  },
  components: {
    'vue-slider': Slider
  },
  computed: {
    bgc () {
      return this.colors.hex;
    }
  }
}
</script>

<style lang="scss">
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

#showColor {
  width: 5em;
  margin: 0px auto;
  height: 5em;
}

#sliderContainer {
  margin: 0px auto;
  width: 25%;
  padding: 3em;
}

h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
