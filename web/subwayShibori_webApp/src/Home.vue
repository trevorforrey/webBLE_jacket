<template>
  <div id="home">
    <h1>Subway Shibori</h1>
    <button v-on:click='connect'>connect</button>
  </div>
</template>

<script>
export default {
  data () {
    return {
      colorService: '0x1234',
      colorWriteCharacteristic: '0x002',
      jacket: {}
    }
  },
  methods: {
    connect: function() {
      console.log('connecting to jacket');


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
