<template>
  <div id="home">
    <h1>Subway Shibori</h1>
    <button v-on:click='connect'>connect</button>
  </div>
</template>

<script>
import {eventBus} from './main.js';
export default {
  data () {
    return {
      colorService: '0x1234',
      colorWriteCharacteristic: '0x002',
      jacket: {},
      writeService: '6e400001-b5a3-f393-e0a9-e50e24dcca9e',
      writeCharacteristic: '6e400002-b5a3-f393-e0a9-e50e24dcca9e',
      colorWriteCharRef: 'nothing'
    }
  },
  methods: {
    connect: function() {
      console.log('connecting to jacket');

      navigator.bluetooth.requestDevice({
        filters: [{
          services: ['6e400001-b5a3-f393-e0a9-e50e24dcca9e']
        }],
        optionalServices: [
          '00003214-0000-1000-8000-00805f9b34fb',
          'ee0c2080-8786-40ba-ab96-99b91ac981d8',
        ]
      })
      .then(device => {
        console.log("device name is: "  + device.name);
        this.jacket = device;
        this.jacket.addEventListener('gattserverdisconnected', this.disconnected);
        return device.gatt.connect();
      })
      .then(server => {
        return server.getPrimaryService(this.writeService);
      })
      .then(service => {
        return service.getCharacteristic(this.writeCharacteristic);
      })
      .then(characteristic => {
        this.colorWriteCharRef = characteristic;
        eventBus.$emit('writeCharRef',this.colorWriteCharRef);
        console.log('grabbed color write char from bluetooth connect');
      })
      .catch(error => {
        console.log('Error in connecting: ', error);
      })
      this.$router.push('color_customization');
    },
    disconnected: function() {
      alert("you have been disconnected");
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
