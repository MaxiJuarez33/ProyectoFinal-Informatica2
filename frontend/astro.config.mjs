// @ts-check
import { defineConfig } from 'astro/config';
import tailwind from '@astrojs/tailwind';

// https://astro.build/config
export default defineConfig({
  integrations: [tailwind()],
  
  // Servidor de desarrollo
  server: {
    port: 4321,
    host: true // Acceso desde otros dispositivos en red local
  },
  
  // Proxy para conectar con tu backend C++
  vite: {
    server: {
      proxy: {
        '/api': {
          target: 'http://localhost:8080',
          changeOrigin: true,
          secure: false
        }
      }
    }
  },
  
  // Optimizaciones para producción
  build: {
    inlineStylesheets: 'auto'
  }
});
