<template>
  <div class="container text-left">
    <h1 class="title">Dashboard</h1>

    <div class="tile is-ancestor">

      <!-- left col -->
      <div class="tile is-vertical">
        <!-- Schedule chart -->
        <div class="tile is-parent">
          <article class="tile is-child notification is-light">
            <div class="content">
              <p class="title">Light Schedule</p>
              <div class="content">
                <schedule-chart></schedule-chart>
              </div>
            </div>
          </article>
        </div>
      </div>

      <!-- Right col -->
      <div class="tile is-vertical">

        <div class="tile">

          <!-- Software status -->
          <div class="tile is-parent">
            <article class="tile is-child notification is-primary">
              <p class="title">Software</p>
              <p class="subtitle">Services info</p>
              <div class="columns">
                <div class="column is-12-mobile">
                  <ul>
                    <li>Time: {{status.upTime}}</li>
                    <li>MQTT Server: {{status.mqttService}}</li>
                    <li>NTP: {{status.ntpService}}</li>
                  </ul>
                </div>
              </div>
            </article>
          </div>

          <!-- Hardware Status -->
          <div class="tile is-parent">
            <article class="tile is-child notification is-info">
              <p class="title">Hardware</p>
              <p class="subtitle">Device info</p>
              <div class="columns">
                <div class="column is-12-mobile">
                  <ul>
                    <li>Chip ID: {{status.chipId}}</li>
                    <li>Free Heap: {{status.freeHeap}}</li>
                    <li>VCC: {{status.vcc}}</li>
                    <li>WIFI: {{wifiCurrentMode}}</li>
                    <li>IP Address: {{status.ipAddress}}</li>
                    <li>MAC: {{status.macAddress}}</li>
                  </ul>
                </div>
              </div>
              <div class="columns">
                <div class="column is-12-mobile">
                  <div class="field is-grouped">
                    <div class="control">
                      <a class="button is-warning" v-on:click="rebootDevice">Restart</a>
                      <p class="help">Reboot device</p>
                    </div>
                    <div class="control">
                      <a class="button is-danger" v-on:click="restoreDevice">Factory Reset</a>
                      <p class="help">Restore initial configuration</p>
                    </div>
                  </div>
                </div>
              </div>
            </article>
          </div>

        </div>


        <!-- Light toggle -->
        <div class="tile is-parent">
          <article class="tile is-child notification is-warning">
            <p class="title">Light</p>
            <p class="subtitle">Channel control</p>
            <div class="columns is-vcentered">
              <div class="column is-12">
                <div class="field is-horizontal" v-for="(led, index) in duty">
                  <div class="field-label is-normal">
                    <label class="label">
                      LED #{{index}}
                      <span class="tag is-hidden-tablet" v-bind:class="{'is-success': led > 0, 'is-dark': led === 0}">
                          {{ledStatus(led)}}
                        </span>
                    </label>
                  </div>
                  <div class="field-body">
                    <div class="field">
                      <div class="control">
                        <span class="tag is-hidden-mobile" v-bind:class="{'is-success': led > 0, 'is-dark': led === 0}">
                          {{ledStatus(led)}}
                        </span>
                      </div>
                    </div>
                    <div class="field">
                      <div class="control">
                        <slider v-model.number="duty[index]" min="0" max="255" v-on:change="setDuty"></slider>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </article>
        </div>
      </div>
    </div>
  </div>
</template>

<script>

import ScheduleChart from '@/components/Ui/ScheduleChart'
import ToggleSwitch from '@/components/Inputs/ToggleSwitch'
import Slider from '@/components/Inputs/InputSlider'
import { store, mutations } from '../store'
import EventBus from '../eventBus'

export default {
  name: 'home',
  components: {
    ToggleSwitch, Slider, ScheduleChart
  },
  data: function () {
    return {
      status: {
        upTime: "",
        chipId: 0,
        freeHeap: 0,
        vcc: 0,
        wifiMode: 0,
        ipAddress: '',
        macAddress: '00:00:00:00:00:00',
        mqttService: '',
        ntpService: '',
        duty: []
      },
      duty: [],
      refreshInterval: null
    }
  },
  computed: {
    wifiCurrentMode () {
      let WiFiMode = ["OFF", "Station", "Access Point", "AP+STA"]
      if (this.status.wifiMode > WiFiMode.length)
        return

      return WiFiMode[this.status.wifiMode];
    }
  },
  methods: {
    ledStatus (value) {
      if (value) { return 'ON ' + parseInt(value * 100 / 255) + '%' } else { return 'OFF ' + parseInt(value * 100 / 255) + '%' }
    },
    getStatus () {
      if (store.status) {
        this.status = store.status
      }
    },
    restoreDevice () {
      EventBus.$emit('erase')
    },
    rebootDevice () {
      EventBus.$emit('reboot')
    },
    setDuty () {
      if (this.duty.length > 0) {
        EventBus.$emit('setDuty', this.duty)
      }

    }
  },
  mounted () {
    EventBus.$on('statusLoaded', id => {
      this.getStatus()
      EventBus.$emit('getDuty')
    })

    EventBus.$on('dutyReady', newDuty => {
      console.log(newDuty)
      if (newDuty.length > 0)
        this.duty = [...newDuty]
    })

    // this.refreshInterval = setTimeout(() => {
    //   EventBus.$emit('getStatus')
    //   console.log(this.refreshInterval)
    // }, 1000)
  },
  destroyed() {
    // clearInterval(this.refreshInterval)
  }
}
</script>

<style scoped>
  .action {
    margin: 8px !important;
  }
  /*@media screen and (max-width: 768px) {*/
    /*.is-fixed-bottom-touch {*/
      /*position: fixed;*/
    /*}*/
  /*}*/

</style>