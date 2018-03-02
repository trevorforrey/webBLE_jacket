<template>
  <div id="colorPicker">
    <h3>Color Picker Page</h3>
    <div id='colors'>
      <div id='color1'></div>
    </div>
    <button v-on:click='sendColor'>Update Color</button>
    <!-- It would be a good idea to create a read characteristic of the current led color
    so show what current vs new is on the page.
    Note: I can bind the color of a div to the data of the last color picked -->
  </div>
</template>

<script>
export default {
  import {eventBus} from './main.js';
  data () {
    return {
      colorService: '0x1234',
      colorWriteCharacteristic: '0x002',
      jacket: {},
      colorWriteCharRef: 'nothing',
      newColor: 'blue'
    }
  },
  methods: {
    sendColor: function() {
      console.log('sending color to ble feather');
      let colorPacket = new Uint8Array(6);
      colorPacket[0] = "!";
      colorPacket[1] = "C";
      colorPacket[2] = "try later";
      return;
    }
  },
  mounted: {
    eventBus.$on('writeCharRef', (characteristic) => {
      console.log('got write characteristic (color picker)');
      this.colorWriteCharRef = characteristic;
    });
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

#colors {
  width: 50%;
  margin: 0px auto;
}

#color1 {
  width: 4em;
  height: 4em;
  background-color: blue;
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
