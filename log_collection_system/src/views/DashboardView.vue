<template>
    <div>
      <h1>仪表盘</h1>
      <p>这里是仪表盘的内容</p>
  
      <div ref="cpuChart" style="width: 100%; height: 400px;"></div>
      <div ref="memoryChart" style="width: 100%; height: 400px; margin-top: 20px;"></div>
      <div ref="systemInfoChart" style="width: 100%; height: 400px; margin-top: 20px;"></div>
    </div>
  </template>
  
  <script setup>
  import { ref, onMounted } from 'vue';
  import * as echarts from 'echarts';
  
  // 用来存储图表实例的引用
  const cpuChart = ref(null);
  const memoryChart = ref(null);
  const systemInfoChart = ref(null);
  
  // 初始化CPU使用率图表
  const initCpuChart = () => {
    const chart = echarts.init(cpuChart.value);
    const option = {
      tooltip: {
        formatter: '{a} <br/>{b} : {c}%',
      },
      series: [
        {
          name: 'CPU使用率',
          type: 'gauge',
          min: 0,
          max: 100,
          splitNumber: 10,
          radius: '80%',
          axisLine: {
            lineStyle: {
              width: 10,
              color: [
                [0.2, '#0f0'],
                [0.8, '#ff0'],
                [1, '#f00'],
              ],
            },
          },
          axisLabel: {
            fontSize: 16,
          },
          detail: {
            valueAnimation: true,
            formatter: '{value}%',
            fontSize: 30,
          },
          data: [{ value: 65, name: 'CPU使用率' }], // 示例数据，可以动态更新
        },
      ],
    };
    chart.setOption(option);
  };
  
  // 初始化内存使用率图表
  const initMemoryChart = () => {
    const chart = echarts.init(memoryChart.value);
    const option = {
      tooltip: {
        formatter: '{a} <br/>{b} : {c}%',
      },
      series: [
        {
          name: '内存使用率',
          type: 'gauge',
          min: 0,
          max: 100,
          splitNumber: 10,
          radius: '80%',
          axisLine: {
            lineStyle: {
              width: 10,
              color: [
                [0.2, '#0f0'],
                [0.8, '#ff0'],
                [1, '#f00'],
              ],
            },
          },
          axisLabel: {
            fontSize: 16,
          },
          detail: {
            valueAnimation: true,
            formatter: '{value}%',
            fontSize: 30,
          },
          data: [{ value: 50, name: '内存使用率' }], // 示例数据，可以动态更新
        },
      ],
    };
    chart.setOption(option);
  };
  
  // 初始化系统信息图表
  const initSystemInfoChart = () => {
    const chart = echarts.init(systemInfoChart.value);
    const option = {
      tooltip: {
        formatter: '{a} <br/>{b} : {c}%',
      },
      series: [
        {
          name: '系统信息',
          type: 'gauge',
          min: 0,
          max: 100,
          splitNumber: 10,
          radius: '80%',
          axisLine: {
            lineStyle: {
              width: 10,
              color: [
                [0.2, '#0f0'],
                [0.8, '#ff0'],
                [1, '#f00'],
              ],
            },
          },
          axisLabel: {
            fontSize: 16,
          },
          detail: {
            valueAnimation: true,
            formatter: '{value}%',
            fontSize: 30,
          },
          data: [{ value: 75, name: '系统负载' }], // 示例数据，可以动态更新
        },
      ],
    };
    chart.setOption(option);
  };
  
  onMounted(() => {
    initCpuChart();
    initMemoryChart();
    initSystemInfoChart();
  });
  </script>
  
  <style scoped>
  /* 这里可以添加自定义样式 */
  </style>
  